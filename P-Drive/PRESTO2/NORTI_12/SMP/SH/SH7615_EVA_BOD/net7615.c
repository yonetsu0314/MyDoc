/****************************************************************************
* NORTi TCP/IP サンプルメインプログラム                                     *
*   MS7615SE, SH7615EV, SH7616EV,                                           *
*   MS7709SE, MS7709ASE, MS7727SE, MS7729RSE,                               *
*   MS7750SE, MS7750SSE  共用                                               *
*                                                                           *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
* 23/Apr/2000 Created                                                       *
* 06/Apr/2000 Supported MS7615SE01 board                                    *
* 25/Feb/2003 char/unsigned char のキャストを追加(ワーニングの削除)      KA *
* 20/Apr/2003 MS7750SE01＋MSWCEDB02へ移植, POL, DUMP機能の追加           SZ *
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

#if defined(SH7615EV)
const char title[] = "\r\n*** NORTi TCP/IP Sample Program for SH7615EV ***";
#include "nos7615.h"
#include "sh7615.h"

#elif defined(SH7616EV)
const char title[] = "\r\n*** NORTi TCP/IP Sample Program for SH7616EV ***";
#include "nos7616.h"
#include "sh7616.h"
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

extern ER dns_resolver(ID cepid, UW dns_ip, B *name, UW* ipaddr);

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

/* 制御ブロック */

T_TELNETD telnetd;      /* TELNETD 制御ブロック */
T_CONSOLE console;      /* Console 制御ブロック */
T_FTP ftp;              /* FTP Server 制御ブロック */
T_FTP ftpc;             /* FTP Client 制御ブロック */
#define NFILE       8   /* 同時オープン可能なファイル数 */
T_FILE file[NFILE];     /* ファイル制御ブロック */
T_DISK disk[1];         /* ディスクドライブ制御ブロック */

/* オブジェクト生成情報 */

TASK MainTask(void);
const T_CTSK cMainTask = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (const B *)"MainTask" };

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
        print((B *)"ip  ping  ftp  telnet  techo  uecho  dns  dhcp");
    else if (stricmp(argv[0], (char *)"ip") == 0)
        disp_param();
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
    else if (stricmp(argv[0], (char *)"techo") == 0)
        tcp_echo_client(0);
    else if (stricmp(argv[0], (char *)"uecho") == 0)
        udp_echo_client(0);
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

int disp_param(void)
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
        print((B *)"[DNS IP Address     ] : [");
        print_ip(dns_ipaddr);
        print((B *)"]\r\n");
        print((B *)"[DHCP IP Address    ] : [");
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
* DNSのテスト
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
* MACアドレスの読み取り
*
* FlashやEEPROMに保存の場合、ここで変数に代入してください。
* LANドライバが対応しており、LANコントローラのEEPROMから取得の場合は、ALL 0
* としてください。
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
    ethernet_addr[0] = 0x12;    /* 暫定的に即値(ローカルなMACアドレス) */
    ethernet_addr[1] = 0x34;
    ethernet_addr[2] = 0x56;
    ethernet_addr[3] = 0x78;
    ethernet_addr[4] = 0x9A;
    ethernet_addr[5] = 0x96;
  #endif
    return TRUE;
}

/*****************************************************************************
* IPアドレス等の読み取り
*
* FlashやEEPROMに保存の場合、ここで変数に代入してください。
* ルータが存在する場合、default_gatewayとsubnet_maskも設定してください。
* DHCPで取得する場合は、これらの変数に上書きされます。
******************************************************************************/

BOOL read_ip_addr(void)
{
    default_ipaddr[0] = 192;    /* 暫定的に即値!! */
    default_ipaddr[1] = 168;
    default_ipaddr[2] = 0;
    default_ipaddr[3] = 96;

    default_gateway[0] = 192;   /* 暫定的に即値!! */
    default_gateway[1] = 168;
    default_gateway[2] = 0;
    default_gateway[3] = 1;

    subnet_mask[0] = 255;       /* 暫定的に即値!! */
    subnet_mask[1] = 255;
    subnet_mask[2] = 255;
    subnet_mask[3] = 0;
    return TRUE;
}

/*****************************************************************************
* メインタスク
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

    /* プロトコルスタック初期化 */
    ercd = tcp_ini();
    if (ercd != E_OK)
        goto END;

    /* コンソール初期化 */
  #if !(defined(DUMP) && CH_DUMP==0)
    ercd = console_ini(&console, 0, 0, 0, (B *)"38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    console_print(&console, (B *)title);
  #endif

    /* DHCPサーバーからIPアドレスを取得 */
  #ifdef DHCP
    dly_tsk(2200/MSEC); /* ハードによってはもっと小さな値も可(10以上) */
    ercd = dhcp_command();
    if (ercd != E_OK)
        read_ip_addr();
  #endif

    ercd = shell_ini(&console, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    /* ファイルシステムと RAM ディスクの初期化 */
    ercd = file_ini(file, NFILE, 0, 0);
    if (ercd != E_OK)
        goto END;

    /* (注) SDRAM の後半 1MB (0x06F00000～0x06FFFFFF) を使用します.  */
    /* この領域に変数やスタックを割り当てないよう注意してください. */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0x06F00000, 0x100000);
    if (ercd != E_OK)
        goto END;

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

    /* FTP サーバー初期化 */
    ercd = ftp_cre_srv(&ftp, 0, 0, 0, 0, ftp_passwd_check);
    if (ercd != E_OK)
        goto END;
    ercd = ftp_sta_srv(&ftp);
    if (ercd != E_OK)
        goto END;

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
* peripheral 初期化の例
*
******************************************************************************/

void ini_pio(void)
{
    sfr_setw(PFC_PBCR2, PBCR_RXD2);  /* to RxD2 */
    sfr_setw(PFC_PBCR2, PBCR_TXD2);  /* to TxD2 */
}

/*****************************************************************************
*  main関数
*
* コンフィグレータを使用する場合は、コンフィグレータが作成したファイルと
* リンクしてください。
******************************************************************************/
#ifndef USE_CONFIG
int main(void)
{
    ID id;

    /* システム初期化 */
    ini_pio();                  /* peripheral 初期化 */
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
