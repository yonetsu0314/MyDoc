/******************************************************************************
* NORTi Network サンプルメインプログラム                                      *
*  (MiNET-7042, MS7615SE, MS7709SE, MS7709ASE, MS7727SE, MS7750SE共用)        *
*                                                                             *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000  Created                                                        *
* 06/Apr/2000  Supported MS7615SE01 board                                     *
* 20/Jun/2001  Supported MS7727SE01 board                                     *
******************************************************************************/

#ifdef MiNET7042
const char title[] = "\r\n*** MiNET-7042 Test Program Ver 2.0 ***";
#endif

#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "nosio.h"

#if defined(MS7709A)
#include "nos7709a.h"
#include "sh7709a.h"
#elif defined(MS7727)
#include "nos7727.h"
#include "sh7727.h"
#elif defined(MS7709)
#include "nos7709.h"
#include "sh7709.h"
#elif defined(MiNET7042)
#include "net7042.h"
#elif defined(MS7750)
#include "nos7750.h"
#include "sh7750.h"
#elif defined(MS7615)
#include "nos7615.h"
#include "sh7615.h"
#endif

#include "nonet.h"
#include "nonecho.h"
#include "nontftp.h"
#include "nonteln.h"
#include "noncons.h"
#include "nonfile.h"
#include "nonftp.h"
#include "nonitod.h"
#include "nondhcp.h"

/* 制御ブロック */

T_TELNETD telnetd;              /* TELNETD 制御ブロック */
T_CONSOLE console;              /* Console 制御ブロック */
T_FTP ftp;                      /* FTP Server 制御ブロック */
T_FTP ftpc;                     /* FTP Client 制御ブロック */
#define NFILE           8       /* 同時オープン可能なファイル数 */
T_FILE file[NFILE];             /* ファイル制御ブロック */
T_DISK disk[1];                 /* ディスクドライブ制御ブロック */

/* オブジェクト生成情報 */

TASK MainTask(void);
const T_CTSK cMainTask = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (B *)"MainTask" };

/*****************************************************************************
* TELNET のパスワードチェック
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
* TELNET のコマンド処理
*
******************************************************************************/

int ping_command(int argc, char *argv[]);
int ftp_command(int argc, char *argv[]);
int dial_command(int argc, char *argv[]);
int telnet_command(int argc, char *argv[]);
int dns_command(int argc, char *argv[]);

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
#ifdef MiNET7042
        print((B *)"test  ping  ftp  telnet  dns  dhcp");
#else
        print((B *)"ip  ping  ftp  telnet  techo  uecho  dns  dhcp");
#endif
    else if (stricmp(argv[0], (char *)"ping") == 0)
        r = ping_command(argc, argv);
    else if (stricmp(argv[0], (char *)"ftp") == 0)
        r = ftp_command(argc, argv);
    else if (stricmp(argv[0], (char *)"telnet") == 0)
        r = telnet_command(argc, argv);
    else if (stricmp(argv[0], (char *)"dns") == 0)
        dns_command(argc, argv);
    else if (stricmp(argv[0], (char *)"dhcp") == 0)
        dhcp_command();
#ifndef MiNET7042
    else if (stricmp(argv[0], (char *)"techo") == 0)
        tcp_echo_client(0);
    else if (stricmp(argv[0], (char *)"uecho") == 0)
        udp_echo_client(0);
    else if (stricmp(argv[0], (char *)"ip") == 0)
        disp_param();
#else
    else if (stricmp(argv[0], (char *)"test") == 0)
        r = test_command(argc, argv);
#endif

    else
        ;   /* 以下、コマンドは適当に作成してください */

    return TRUE;
}


/*****************************************************************************
* 表示補助関数
*
******************************************************************************/

static void print_ip(UB *ipaddr)
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

static void print_mac(UB *macaddr)
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

void error(char *err_str, ER ercd)
{
    char str[10];

    ito_d(str, (short)ercd, 3);
    print((B *)err_str);
    print((B *)":(");
    print((B *)str);
    print((B *)")\r\n");
}

/*****************************************************************************
* 設定パラメータの表示
*
******************************************************************************/

int disp_param()
{
    print((B *)"[Ethernet Address   ] : [");
    print_mac(ethernet_addr);
    print((B *)"]\r\n");
    print((B *)"[Default IP Address ] : [");
    print_ip(default_ipaddr);
    print((B *)"]\r\n");
    print((B *)"[Default Gateway    ] : [");
    print_ip(default_gateway);
    print((B *)"]\r\n");
    print((B *)"[Subnet Mask        ] : [");
    print_ip(subnet_mask);
    print((B *)"]\r\n");
    return 1;
}

/*****************************************************************************
* DHCPのテスト
*
******************************************************************************/
extern T_TERMINAL *current_terminal;

ER dhcp_command()
{
    ER ercd;

    current_terminal = (T_TERMINAL *)&console;

    ercd = dhcp_get_data(0);
    if (ercd != E_OK)
        error((char *)"\r\n\r\nError get data from DHCP server.", ercd);
    else {
        print((B *)"\r\nSuccess get data from DHCP server.\r\n");
        disp_param();
        print((B *)"[DNS IP Address     ] : [");
        print_ip(dns_ipaddr);
        print((B *)"]\r\n");
        print((B *)"[DHCP IP Address    ] : [");
        print_ip(dhcp_ipaddr);
        print((B *)"]\r\n");
    }
    current_terminal = NULL;

    return ercd;
}

/*****************************************************************************
* DNSのテスト
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

    if (dns_ip == 0 ){
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

/* MiNET-7042ではEthenet AddressとIP AddressをEEPROMから取得します */

#ifndef MiNET7042
/*****************************************************************************
* Ethernet Address の入力
*
******************************************************************************/

BOOL read_ethernet_addr(void)
{
    ethernet_addr[0] = 0x12;    /* 暫定的に即値!! */
    ethernet_addr[1] = 0x34;
    ethernet_addr[2] = 0x56;
    ethernet_addr[3] = 0x78;
    ethernet_addr[4] = 0x9a;
    ethernet_addr[5] = 0x99;
    return TRUE;
}

/*****************************************************************************
* IP Address の入力
*
******************************************************************************/

BOOL read_ip_addr(void)
{
    default_ipaddr[0] = 192;    /* 暫定的に即値!! */
    default_ipaddr[1] = 168;
    default_ipaddr[2] = 0;
    default_ipaddr[3] = 99;

/* ルータが存在する場合に、以下を有効にしてください。 */
    default_gateway[0] = 192;   /* 暫定的に即値!! */
    default_gateway[1] = 168;
    default_gateway[2] = 0;
    default_gateway[3] = 1;

    subnet_mask[0] = 255;    /* 暫定的に即値!! */
    subnet_mask[1] = 255;
    subnet_mask[2] = 255;
    subnet_mask[3] = 0;
    return TRUE;
}
#endif

/*****************************************************************************
* メインタスク
*
******************************************************************************/

TASK MainTask(void)
{
    ER ercd;
    INT i;

    /* シリアルへ LAN パケット内容を出力 */

  #ifdef DUMP
    ercd = ini_sio(1, "38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    ercd = ctl_sio(1, TSIO_RXE|TSIO_TXE|TSIO_RTSON);
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

    /* プロトコルスタック初期化 */

    ercd = tcp_ini();
    if (ercd != E_OK)
        goto END;

    /* コンソール初期化 */

    ercd = console_ini(&console, 0, 0, 0, (B *)"9600 B8 PN S1 XON");
    if (ercd != E_OK)
        goto END;


    /* DHCPサーバーからIPアドレスを取得 */
  #ifdef DHCP
    ercd = dhcp_command();
    if (ercd != E_OK)
        read_ip_addr();
  #endif

    /* ファイルシステムと RAM ディスクの初期化 */

    ercd = file_ini(file, NFILE, 0, 0);
    if (ercd != E_OK)
        goto END;

  #if (defined(MS7709A) || defined(MS7709) || defined(MS7727) || defined(MS7750))
    /* (注) EDO-DRAM の後半 1MB (AC100000～AC1FFFFF) を使用します.  */
    /* この領域に変数やスタックを割り当てないよう注意してください. */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0xac100000, 0x100000);
    if (ercd != E_OK)
        goto END;
  #elif defined(MiNET7042)
    /* (注) SRAM の後半 512KB (0x480000～0x4FFFFF) を使用します.  */
    /* この領域に変数やスタックを割り当てないよう注意してください. */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0x480000, 0x80000);
    if (ercd != E_OK)
        goto END;
  #elif defined(MS7615)
    /* (注) SDRAM の後半 1MB (0x06F00000～0x06FFFFFF) を使用します.  */
    /* この領域に変数やスタックを割り当てないよう注意してください. */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0x06F00000, 0x100000);
    if (ercd != E_OK)
        goto END;
  #endif

    /* UDP エコーサーバー初期化 */

    ercd = udpecho_ini(0, 0);
    if (ercd != E_OK)
        goto END;

    /* TFTP サーバー初期化 */

    ercd = tftpsrv_ini(0, 0);
    if (ercd != E_OK)
        goto END;

    /* TCP エコーサーバー初期化 */

    ercd = tcpecho_ini(0, 0, 0);
    if (ercd != E_OK)
        goto END;

    /* TELNET デーモン初期化 */

    ercd = telnetd_ini(&telnetd, 0, 0, 0, 0);
    if (ercd != E_OK)
        goto END;
    ercd = shell_ini(&telnetd, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    ercd = shell_ini(&console, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    /* FTP サーバー初期化 */

    ercd = ftp_cre_srv(&ftp, 0, 0, 0, 0, NULL);
    if (ercd != E_OK)
        goto END;
    ercd = ftp_sta_srv(&ftp);
    if (ercd != E_OK)
        goto END;

END:
    for (;;)
        slp_tsk();
}

#if (defined(MS7709A) || defined(MS7727) || defined(MS7750) || defined(MS7615))
/*****************************************************************************
* SuperIO の初期化
*
******************************************************************************/

#if defined(MS7615)
#define  CONF_BASE  (*(volatile unsigned short *) 0x22000700)
#define  CONF_PORT  (*(volatile unsigned short *) 0x220007E0)
#define  INDEX_PORT (*(volatile unsigned short *) 0x220007E0)
#define  DATA_PORT  (*(volatile unsigned short *) 0x220007E2)
#endif

/*****************************************************************************
* peripheral 初期化の例
*
******************************************************************************/

void ini_pio(void)
{

 #if defined(MS7615)
    sfr_setw(PFC_PBCR, PBCR_RXD1);  /* to RxD1 */
    sfr_setw(PFC_PBCR, PBCR_TXD1);  /* to TxD1 */
    sfr_setw(PFC_PBCR, PBCR_RTS1);  /* to ~RTS */
    sfr_setw(PFC_PBCR, PBCR_CTS1);  /* to ~CTS */

    sfr_setw(PFC_PBCR2, PBCR_RXD2);  /* to RxD2 */
    sfr_setw(PFC_PBCR2, PBCR_TXD2);  /* to TxD2 */
 #endif
}
#endif /* (defined(MS7709A) || defined(MS7727) || defined(MS7750) || defined(MS7615)) */

/*****************************************************************************
*  main関数
*
*  コンフィグレータを使用する場合は、
*  コンフィグレータが作成したファイルとリンクしてください。
******************************************************************************/
#ifndef USE_CONFIG
int main(void)
{
    ID id;

    /* システム初期化 */
  #ifdef MiNET7042
    ini_port();                 /* ポート初期化 */
    ini_eeprom();               /* EEPROM ポート初期化 */
  #elif (defined(MS7709A) || defined(MS7727) || defined(MS7750) || defined(MS7615))
    ini_pio();                  /* peripheral 初期化 */
  #endif
    sysini();

    /* タスク生成 */

    id = acre_tsk(&cMainTask);

    /* タスク起動 */

    sta_tsk(id, 0);

    /* システム起動 */

    intsta();                   /* 周期タイマ割込みを起動 */
    syssta();                   /* マルチタスクへ移行 */
}
#endif

/* end */
