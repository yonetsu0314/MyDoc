/****************************************************************************
* NORTi Network Sample Program for MS7751SE01                               *
*                                                                           *
*  Copyright (c) 1995-2002, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
* 13/Aug/2001  Supported MS7751SE01 board                                   *
* 08/Apr/2002  RAM Disk Format added                            [SP]        *
* 20/Oct/2002  Board specific header file (ms7751.h) added      [SP]        *
* 13/Dec/2002  glb_func.h file removed                          [SP]        *
* 25/Feb/2003 char/unsigned char のキャストを追加(ワーニングの削除)      KA *
* 21/Apr/2003 MiNET-7042を除外, 各ボード用の起動メッセージ(title)追加    MI *
* 22/Apr/2003 MACアドレス設定コマンド mac_command 追加                   MI *
* 07/May/2003 dhcp_command実行後にコンソールの入力が無効になる問題を修正 YS *
* 07/May/2003 ftp_passwd_checkを追加                                     YS *
* 12/May/2003 上四つの修正を他のnet7xxx.cに適用                          KA *
* 18/Jun/2003 sioの設定を"38400 B8 PN S1"に統一、dns_resolverプロトタイプ   *
*             CH_DUMPの追加                                              KA *
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "nosio.h"

#include "sh7751.h"
#include "ms7751.h"

#include "nonet.h"
#include "nonecho.h"
#include "nontftp.h"
#include "nonteln.h"
#include "noncons.h"
#include "nonfile.h"
#include "nonftp.h"
#include "nonitod.h"
#include "nondhcp.h"

extern ER dns_resolver(ID cepid, UW dns_ip, B *name, UW* ipaddr);

#ifdef MSWCEDB02
ER lan_get_mac(UB *macaddr);
ER lan_set_mac(UB *macaddr);
#else
ER lan_get_mac(UB *macaddr) { return -1; }
ER lan_set_mac(UB *macaddr) { return -1; }
#endif

/* デバッグ用 */

#ifndef POL
#define POL         0   /* LANドライバで割込みを使う(0), ポーリングでテスト(1) */
#endif
#if POL
void lan_intr(void);
#endif

#ifndef CH_DUMP
#define CH_DUMP     1   /* LANパケットダンプ用のシリアルチャネル番号 */
#endif

#ifndef CH
#define CH  0
#endif

/* 制御ブロック */

T_TELNETD telnetd;      /* TELNETD 制御ブロック */
T_CONSOLE console;      /* Console 制御ブロック */
T_FTP ftp;              /* FTP Server 制御ブロック */
T_FTP ftpc;             /* FTP Client 制御ブロック */
#define NFILE       8   /* 同時オープン可能なファイル数 */
T_FILE file[NFILE];     /* ファイル制御ブロック */
T_DISK disk[1];         /* ディスクドライブ制御ブロック */

/* オブジェクト生成情報 */

ID IDMain_Task;
TASK MainTask(void);
const T_CTSK cMainTask = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (const B *)"MainTask" };


const char title[] = "\n\n\r\
------------------------------------------------------------------------\r\n\
          NORTi Version 4 Sample Demonstration program                  \r\n\
                             for                                        \r\n\
               MS7751SE01 Hitachi Evaluation board                      \r\n\
                     Controller - SH7751                                \r\n\
                                                                        \r\n\
Program version 2.0                                                     \r\n\
------------------------------------------------------------------------\r\n";

const char msg_tcpini[] =
"\n\tEthernet controller and Network Protocol stack initialized....\r";
const char msg_fileini[] = "\r\n\tFile System initialized....\r";
const char msg_diskini[] = "\r\n\tRAM Disk (1MB size) initialized....\r";
const char msg_diskformat[] = "\r\n\tRAM Disk formatted....\r";
const char msg_initerr[] = "\r\n\tNetwork Stack initialization error....\r";
const char msg_UDPEini[] = "\r\n\tUDP Echo Server initialized....\r";
const char msg_TFTPini[] = "\r\n\tTFTP Server initialized....\r";
const char msg_TCPEini[] = "\r\n\tTCP Echo Server initialized....\r";
const char msg_TELNETDini[] = "\r\n\tTELNET Demon initialized....\r";
const char msg_FTPSini[] = "\r\n\tFTP Server initialized....\r\n";
const char msg_PCIini_err[] = "\r\n\tPCI Bus Initialization error.. \r\n\
Please restart hardware....\r\n";


/*****************************************************************************
* Message display using serial console.
*
******************************************************************************/
static void print_msg(const char *c)
{

    UB *p;
    p = (UB *)c;

    while ((*p++) !=0) {
        put_sio(CH, (*p), 10/MSEC);
    }
}

/*****************************************************************************
* FTPd のパスワードチェック
*
******************************************************************************/

B *ftp_passwd_check(T_FTP *ftp, const B *user, const B *pass)
{
  #if 0
    if ((strcmp(user, "mispo") == 0)
     && (strcmp(pass, "12345") == 0))
        return "230 Logged in\r\n";
    else
        return NULL;
  #else
    return "230 Logged in\r\n";
  #endif
}

/*****************************************************************************
* Routine to check TELNET Login Password.
*
******************************************************************************/

B *telnet_passwd_check(T_TELNETD *t, const B *name, const B *passwd)
{
  #if 0
    if (strcmp(name, "mispo") == 0
     && strcmp(passwd, "12345") == 0)
        return (B *)"mispo>";
    else
        return (B *)NULL;
  #else
    return (B *)">";
  #endif
}

/*****************************************************************************
* TELNET command processing routines.
*
******************************************************************************/

int ping_command(int argc, char *argv[]);
int ftp_command(int argc, char *argv[]);
int dial_command(int argc, char *argv[]);
int telnet_command(int argc, char *argv[]);
int dns_command(int argc, char *argv[]);
int mac_command(int argc, char *argv[]);
int disp_param(void);
ER dhcp_command(void);

BOOL telnetd_callback(T_TERMINAL *t, B *s)
{
    char *argv[16];
    int argc, r;

    for (argc = 0; argc < sizeof argv / sizeof (B *); argc++)
    {   while (*s == ' ')
            s++;
        if (*s == '\0')
            break;
        argv[argc] = (char *)s;
        s = (B *)strchr((char *)s, ' ');
        if (s == NULL)
            break;
        *s++ = '\0';
    }
    argc++;

    if (stricmp(argv[0], (char *)"?") == 0 || stricmp(argv[0], (char *)"help") == 0)
        print((B *)"ip  ping  ftp  telnet  techo  uecho  dns  dhcp  mac");
                                            /* Command Menu Screen */
    else if (stricmp(argv[0], (char *)"ip") == 0)
        disp_param();                       /* Display IP configuration */
    else if (stricmp(argv[0], (char *)"ping") == 0)
        r = ping_command(argc, argv);       /* Execute Ping Command */
    else if (stricmp(argv[0], (char *)"ftp") == 0)
        r = ftp_command(argc, argv);        /* Execute FTP command */
    else if (stricmp(argv[0], (char *)"telnet") == 0)
        r = telnet_command(argc, argv);     /* Execute TELNET command */
    else if (stricmp(argv[0], (char *)"dns") == 0)
        dns_command(argc, argv);            /* Execute DNS command */
    else if (stricmp(argv[0], (char *)"dhcp") == 0)
        dhcp_command();                     /* Execute DHCP command */
    else if (stricmp(argv[0], (char *)"techo") == 0)
        tcp_echo_client(0);                 /* Execute TCP echo command */
    else if (stricmp(argv[0], (char *)"uecho") == 0)
        udp_echo_client(0);                 /* Execute UDP echo command */
    else if (stricmp(argv[0], (char *)"mac") == 0)
        mac_command(argc, argv);
    else
        ;   /* Please create the customized commands here... */

    return TRUE;
}

/*****************************************************************************
* Supplementary Display Functions.
*
******************************************************************************/

static void print_ip(UB *ipaddr)    /* Function block to display IP  address */
{
    B  buf[10];
    INT i;

    for (i = 0 ; ; i++) {
        itou((char *)buf, (UH)ipaddr[i], itoulen(ipaddr[i]));
        print(buf);
        if (i >= 3)
            break;
        print((B *)".");
    }
}

static void print_mac(UB *macaddr)  /* Function block to display Ethernet addr */
{
    B  buf[10];
    INT i;

    for (i = 0 ; ; i++) {
        ito0X((char *)buf, (UH)macaddr[i], 2);
        print(buf);
        if (i >= 5)
            break;
        print((B *)"-");
    }
}

void error(char *err_str, ER ercd)  /* Function block to diaplay error message */
{
    char str[10];

    ito_d(str, (short)ercd, 3);
    print((B *)err_str);
    print((B *)":(");
    print((B *)str);
    print((B *)")\r\n");
}

/*****************************************************************************
* Routine to display IP configuration set parameters.
*
******************************************************************************/

int disp_param(void)
{
    /* This block displays IP configuration as --
            Ethernet address = XXXXXXXXXXXX
            IP Address = XX.XX.XX.XX
            Gateway Address = XX.XX.XX.XX
            Subnet Address = XX.XX.XX.XX
    */

    print((B *)"\t[Ethernet Address   ] : [");
    print_mac(ethernet_addr);
    print((B *)"]\r\n");
    print((B *)"\t[Default IP Address ] : [");
    print_ip(default_ipaddr);
    print((B *)"]\r\n");
    print((B *)"\t[Default Gateway    ] : [");
    print_ip(default_gateway);
    print((B *)"]\r\n");
    print((B *)"\t[Subnet Mask        ] : [");
    print_ip(subnet_mask);
    print((B *)"]\r\n");
    return 1;
}

/*****************************************************************************
* DHCP Command Execution Routine.
*
******************************************************************************/

extern T_TERMINAL *current_terminal;

ER dhcp_command(void)
{
    static int dhcp=FALSE;
    ER ercd;
    T_TERMINAL *ct;

    ct = current_terminal;

    if (ct == NULL)
        current_terminal = (T_TERMINAL *)&console;

    if (dhcp)
        ercd = dhcp_reb_data(0);
    else
        ercd = dhcp_get_data(0);

    if (ercd != E_OK)
        error((char *)"\r\n\r\nError get data from DHCP server.", ercd);
    else {
        print((B *)"\r\nSuccess get data from DHCP server.\r\n");
        disp_param();
        print((B *)"\t[DNS IP Address     ] : [");
        print_ip(dns_ipaddr);
        print((B *)"]\r\n");
        print((B *)"\t[DHCP IP Address    ] : [");
        print_ip(dhcp_ipaddr);
        print((B *)"]\r\n");
        if (!dhcp) {
            ercd = dhcp_sta_tsk(0, 0, 0, NULL);
            if (ercd != E_OK)
                error((char *)"\r\n\r\nError dhcp_sta_tsk", ercd);
            dhcp = TRUE;
        }
    }
    current_terminal = ct;

    return ercd;
}

/*****************************************************************************
* DNS Command Execution Routine.
*
******************************************************************************/

int dns_command(int argc, char *argv[])
{
    ER ercd;
    UW dns_ip, ipaddr;
    B buf[20];

    if (argc < 2) {
        print((B *)"Usage: dns domainname [dns server address]\r\n");
        return 0;
    }

    dns_ip = byte4_to_long(dns_ipaddr);
    if (dns_ip == 0)
        dns_ip = ascii_to_ipaddr((B *)argv[2]);

    if (dns_ip == 0){
        print((B *)"DNS server address isn't specified\r\n");
        print((B *)"Usage: dns domainname dns-server-address\r\n");
        return 0;
    }

    ercd = dns_resolver(0, dns_ip, argv[1], &ipaddr);
    if (ercd == E_OK){
        print((B *)"[");
        print((B *)argv[1]);
        print((B *)"] : [");
        long_to_byte4((UB *)buf, ipaddr);
        print_ip((UB *)buf);
        print((B *)"]\r\n");
    }
    else{
        error((char *)"Error dns_resolver", ercd);
        return 0;
    }
    return 1;
}

/*****************************************************************************
* MACアドレスの書き換え
*
* LANドライバが対応しており(lan_get_mac, lan_set_mac関数が実装されており)、
* LANコントローラのEEPROMからMACアドレスを取得する場合にしか有効でありません。
* 本コマンド実行後は、ハードウェアをリセットして再起動してください。
*
* >mac                  現在のEEPROMのMACアドレスの読み出し
* >mac xxxxxxxxxxxx     12桁の16進数で指定されたMACアドレスの書き込み
******************************************************************************/

int mac_command(int argc, char *argv[])
{
    UB data[6];
    char hex[3];
    int c, i;
    char *p;
    ER ercd;

    memset(data, 0xff, 6);

    if (argc < 2) {
        if (lan_get_mac(data) != E_OK)
            error("Error lan_get_mac", ercd);
        print_mac(data);
        return 1;
    }

    p = argv[1];
    for (i = 0; i < 6;) {
        c = *p++;
        if (c == '\0')
            break;
        if (!isxdigit(c))
            continue;
        hex[0] = (B)c;
        c = *p++;
        if (c == '\0')
           break;
        hex[1] = (char)c;
        hex[2] = '\0';
        data[i++] = (UB)xtoi(hex);
    }
    if (i != 6)
        return 0;
    if (lan_set_mac(data) != E_OK) {
        error("Error lan_set_mac", ercd);
        return 0;
    }
    return 1;
}

/*****************************************************************************
* Ethernet Address Defined here.
*
******************************************************************************/

BOOL read_ethernet_addr(void)
{
  #ifdef MSWCEDB02
    ethernet_addr[0] = 0x00;    /* 0ならLANコントローラのEEPROMから取得 */
    ethernet_addr[1] = 0x00;
    ethernet_addr[2] = 0x00;
    ethernet_addr[3] = 0x00;
    ethernet_addr[4] = 0x00;
    ethernet_addr[5] = 0x00;
  #else
    /* Dummy Ethernet address */
    ethernet_addr[0] = 0x12;
    ethernet_addr[1] = 0x34;
    ethernet_addr[2] = 0x56;
    ethernet_addr[3] = 0x78;
    ethernet_addr[4] = 0x9A;
    ethernet_addr[5] = 0x91;
  #endif
    return TRUE;
}

/*****************************************************************************
* IP Address defined here.
*
******************************************************************************/

BOOL read_ip_addr(void)
{
    default_ipaddr[0] = 192;
    default_ipaddr[1] = 168;
    default_ipaddr[2] = 0;
    default_ipaddr[3] = 91;

    default_gateway[0] = 192;
    default_gateway[1] = 168;
    default_gateway[2] = 0;
    default_gateway[3] = 1;

    subnet_mask[0] = 255;
    subnet_mask[1] = 255;
    subnet_mask[2] = 255;
    subnet_mask[3] = 0;
    return TRUE;
}

/*****************************************************************************
* Main Task Functional Block.
*
******************************************************************************/

TASK MainTask(void)
{
    ER ercd;

    /* シリアルへ LAN パケット内容を出力 */
  #ifdef DUMP
    ercd = ini_sio(CH_DUMP, "38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    ercd = ctl_sio(CH_DUMP, TSIO_RXE|TSIO_TXE|TSIO_RTSON);
    if (ercd != E_OK)
        goto END;
    ercd = landump_ini(0, 0, 0);
    if (ercd != E_OK)
        goto END;
  #endif

    /* IP & Ethernet Address の読み出し */
  #ifndef DHCP
    read_ip_addr();
  #endif
    read_ethernet_addr();

    /* TCP/IP protocol stack Initialization */
    ercd = tcp_ini();
    if (ercd != E_OK)
        goto END;
    print_msg(msg_tcpini);

    /* コンソール初期化 */
  #if !(defined(DUMP) && CH_DUMP==CH)
    ercd = console_ini(&console, 0, 0, CH, (B *)"38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    print_msg(title);
  #endif

    led_set(LED_LAN_INIT);          /* LAN Driver init-OK  LED indicator ON */

    /* Get IP address from DHCP server */
  #ifdef DHCP
    dly_tsk(2200/MSEC); /* ハードによってはもっと小さな値も可(10以上) */
    ercd = dhcp_command();
    if (ercd != E_OK)
        read_ip_addr();
  #endif

    ercd = shell_ini(&console, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    /* File System Initialization here.. */
    ercd = file_ini(file, NFILE, 0, 0);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_fileini);

    /* Disk Initialization in SDRAM space 0x8c500000 to 0x8c600000. Size = 1MB */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0x8c500000, 0x100000);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_diskini);
    ercd = my_dformat((char *)"A:"); /* Format the Disk */
    print_msg(msg_diskformat);

    /* UDP Echo Server Initialization */
    ercd = udpecho_ini(0, 0);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_UDPEini);

    /* TFTP Server Initialization */
    ercd = tftpsrv_ini(0, 0);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_TFTPini);

    /* TCP Echo Server Initialization */
    ercd = tcpecho_ini(0, 0, 0);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_TCPEini);

    /* TELNET Demon Initialization */
    ercd = telnetd_ini(&telnetd, 0, 0, 0, 0);
    if (ercd != E_OK)
        goto END;
    ercd = shell_ini(&telnetd, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_TELNETDini);

    /* FTP Server Initialization */
    ercd = ftp_cre_srv(&ftp, 0, 0, 0, 0, ftp_passwd_check);
    if (ercd != E_OK)
        goto END;
    ercd = ftp_sta_srv(&ftp);
    if (ercd != E_OK)
        goto END;
    print_msg(msg_FTPSini);

END:
    for (;;) {
      #if (POL) /* ポーリングでデバッグの場合 */
        chg_pri(TSK_SELF, 1);
        lan_intr();
        chg_pri(TSK_SELF, TPRI_INI);
      #else
        slp_tsk();
      #endif
    }
}

/*****************************************************************************
* peripheral Initialization here..
*
******************************************************************************/

void pci_check(void);   /* PCI bus driver upload */

void ini_pio(void)
{
    UW cnt;
    led_set(LED_ALL);
    for (cnt=60000; cnt > 0; cnt--);
    led_clr(LED_ALL);

    pci_check();            /* PCI bus driver upload */
    for (cnt=60000; cnt > 0; cnt--);
    init_sio_device();      /* Initialize Super IO controller */
    if (init_mc1543() != E_OK) {
        led_set(LED_BOARD_ERROR);
        print_msg(msg_PCIini_err);
        for (;;);           /* error hold - Error during board Initialization */
                            /* RESET the board power to start again */
    }
    led_set(LED_SIO_INIT);
    init_lan_device();      /* Initialize on-board LAN controller */

    /*
        .... Add Customized Device Initialization program here...
    */
}

/*****************************************************************************
*  Main Function
*
******************************************************************************/
#ifndef USE_CONFIG
int main(void)
{
    ID id;

    sysini();                   /* Initialize System before Peripheral Init */
    ini_pio();                  /* Initialize Peripheral Devices */

    id = acre_tsk(&cMainTask);  /* Create Main Task */

    IDMain_Task = id;
    sta_tsk(id, 0);             /* Start Main Task */

    intsta();                   /* Start Cycle Timer Interrupt */
    syssta();                   /* Multitasking start here */
}
#endif

/******************************* End Of File **********************************/
