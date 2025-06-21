/******************************************************************************
* MiNET-7042 ボードテスト処理                                                 *
*                                                                             *
*  Copyright (c) 1998-1999, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 08/Mar/1999 1.6 net7042.c から分離                                          *
* 23/Mar/1999 1.7 Port や CPU の初期化部分を net7042.c から                   *
* 04/Mar/1999 1.8 RTS,CTS 初期化漏れ修正                                      *
* 21/Jun/1999 2.0 サブネットマスク設定の追加                                  *
* 13/Dec/2002 sprintf, sscanf 文を削除                                     KA *
* 24/Dec/2002 sh7042.h->sh7040.h, nos7040.h->nos7040.hに                   KA *
* 27/Dec/2002 volatile 漏れ                                                KA *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "norti3.h"
#include "nonet.h"
#include "nonitod.h"
#include "nonteln.h"
#include "noncons.h"
#include "nonecho.h"
#include "sh7040.h"
#include "nos7040.h"
#include "net7042.h"

/* USE_FORMAT を定義すると sprintf, sscanf 文を使用する *
/* #define USE_FORMAT */

/* 外部参照 */

extern const char title[];

/* 保存するパラメータ */

B phone_number[15+1];           /* PPP 用の電話番号 */
B login_name[15+1];
B login_password[15+1];

/* ボード上のスイッチ/LED */

#define SW_IN   *((volatile UB *)0x00ce0000L)    /* MiNET-7042 の デップスイッチ */
#define LAN_LED *((volatile UB *)0x00cc0021L)    /* MiNET-MB86964 の LED */
#define LED     *((volatile UB *)IO_BASE+IO_PEDR)/* MiNET-7042 の LED */

/* フラッシュメモリのセクタ定義 */

const UW fls_sector[7][2] = {
    0x00000, 0x10000,
    0x10000, 0x10000,
    0x20000, 0x10000,
    0x30000, 0x08000,
    0x38000, 0x02000,
    0x3a000, 0x02000,
    0x3c000, 0x04000
};

/* 内部変数 */

static B ctrl_c;

/*****************************************************************************
* CPU 初期化
*
******************************************************************************/

void ini_mpu(void)
{
    /* CS0 2wait, CS1 0wait, CS2 1wait, CS3 1wait */

    sfr_outw(BSC_WCR1,0x1102);

    /* CS3,CS1 0アイドルサイクル, CS2 CS0 1アイドルサイクル */
    /* 全/CS アサート拡張あり */

    sfr_outw(BSC_BCR2,0x115f);

    /* IRQ7, WAIT 端子許可 */

    sfr_outw(PFC_PBCR1,0x0007);
}

/*****************************************************************************
* ポート初期化
*
******************************************************************************/

#define PE_RTS0     0x0002      /* PE1 を チャネル0 RTS 信号端子とする */
#define PE_RTS1     0x0008      /* PE3 を チャネル1 RTS 信号端子とする */

void ini_port(void)
{
    sfr_outw(IO_PEDR,  0x0000);     /* LED用 PE8..15 初期値 */
    sfr_outw(PFC_PEIOR, 0xffe0);    /* LED用 PE8..15 出力設定 */

    sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0);
    sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0);
    sfr_outw(PFC_PACRL2, 0xad75);
    sfr_setw(IO_PEDR, PE_RTS0|PE_RTS1);
    sfr_setw(PFC_PEIOR, PE_RTS0|PE_RTS1);
    sfr_clrw(PFC_PECR2, 0xa0cc);

    ini_eeprom();
}

/*****************************************************************************
* RTS 信号ON/OFF
*
******************************************************************************/

void rts0_on(void)  { sfr_clrw(IO_PEDR, PE_RTS0); } /* チャネル0 RTS 信号 ON */
void rts0_off(void) { sfr_setw(IO_PEDR, PE_RTS0); } /* チャネル0 RTS 信号 OFF */
void rts1_on(void)  { sfr_clrw(IO_PEDR, PE_RTS1); } /* チャネル1 RTS 信号 ON */
void rts1_off(void) { sfr_setw(IO_PEDR, PE_RTS1); } /* チャネル1 RTS 信号 OFF */

/*****************************************************************************
* LED 点滅テスト
*
******************************************************************************/

void led_test(void)
{
    B s[16+1];
    UB c;

    print((B *)"LED output = ");

    for (;;)
    {
        c = LED;
        if ((c <<= 1) == 0)
            c = 0x01;
        LED = c;

        s[0] = (B)((c & 0x80) ? '1': '0');
        s[1] = (B)((c & 0x40) ? '1': '0');
        s[2] = (B)((c & 0x20) ? '1': '0');
        s[3] = (B)((c & 0x10) ? '1': '0');
        s[4] = (B)((c & 0x08) ? '1': '0');
        s[5] = (B)((c & 0x04) ? '1': '0');
        s[6] = (B)((c & 0x02) ? '1': '0');
        s[7] = (B)((c & 0x01) ? '1': '0');
        memset(s + 8, '\b', 8);
        s[16] = '\0';
        print(s);

        if (kbhit())
        {   getch();
            break;
        }
        dly_tsk(250/MSEC);
    }
    puts((B *)"");
}

/*****************************************************************************
* DIP switch 入力テスト
*
******************************************************************************/

void dip_test(void)
{
    B s[16+1];
    UB c;

    print((B *)"DIP switch = ");

    for (;;)
    {
        c = SW_IN;
        LED = c;

        s[0] = (B)((c & 0x80) ? '1': '0');
        s[1] = (B)((c & 0x40) ? '1': '0');
        s[2] = (B)((c & 0x20) ? '1': '0');
        s[3] = (B)((c & 0x10) ? '1': '0');
        s[4] = (B)((c & 0x08) ? '1': '0');
        s[5] = (B)((c & 0x04) ? '1': '0');
        s[6] = (B)((c & 0x02) ? '1': '0');
        s[7] = (B)((c & 0x01) ? '1': '0');
        memset(s + 8, '\b', 8);
        s[16] = '\0';
        print(s);

        if (kbhit())
        {   getch();
            break;
        }
        dly_tsk(250/MSEC);
    }
    puts((B *)"");
}

/*****************************************************************************
* RTC テスト
*
******************************************************************************/

void rtc_test(void)
{
    B buf[32];
    char *s, *p;
    int i;
    struct tm t;
    struct tm tt;

    memset(&t, 0, sizeof t);
    memset(&tt, -1, sizeof tt);

    for (;;)
    {
        gettime(&t);
#ifdef USE_FORMAT   /* sprintf を使用する例 */
        sprintf(buf, (char *)"%02d-%02d-%02d %02d:%02d:%02d",
                t.tm_year, t.tm_mon + 1, t.tm_mday,
                t.tm_hour, t.tm_min, t.tm_sec);
#else
        s = lto0u((char*)buf, t.tm_year, 2);
        *s++ = '-';
        s = lto0u(s,   t.tm_mon + 1, 2);
        *s++ = '-';
        s = lto0u(s,   t.tm_mday, 2);
        *s++ = ' ';
        s = lto0u(s, t.tm_hour, 2);
        *s++ = ':';
        s = lto0u(s,   t.tm_min, 2);
        *s++ = ':';
        s = lto0u(s,   t.tm_sec, 2);
#endif
        if (memcmp(&tt, &t, sizeof t) != 0)
        {   print(buf);
            print((B *)"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            memcpy(&tt, &t, sizeof t);
        }
        dly_tsk(100/MSEC);

        if (kbhit())
        {   puts((B *)"");
            gets(buf);
            if (buf[0] == '\0')
                break;
            memset(&t, 0, sizeof t);
#ifdef USE_FORMAT   /* sprintf を使用する例 */
            i = sscanf(buf, (char *)"%d-%d-%d %d:%d:%d",
                    &t.tm_year, &t.tm_mon, &t.tm_mday,
                    &t.tm_hour, &t.tm_min, &t.tm_sec);
            if (i != 6)
                continue;
#else
            t.tm_year = strtol(p = (char *)buf, &s, 10 );
            if (s == p || *s++ != '-')continue;
            t.tm_mon = strtol( p = s, &s, 10 );
            if (s == p || *s++ != '-')continue;
            t.tm_mday = strtol( p = s, &s, 10 );
            if (s == p || *s++ != ' ')continue;

            t.tm_hour = strtol( p = s, &s, 10 );
            if (s == p || *s++ != ':')continue;
            t.tm_min = strtol( p = s, &s, 10 );
            if (s == p || *s++ != ':')continue;
            t.tm_sec = strtol( p = s, &s, 10 );
            if (s == p)continue;
#endif
            t.tm_mon -= 1;
            if (settime(&t))
            {   puts((B *)"OK");
                memset(&tt, -1, sizeof tt);
            }
        }
    }
}

/*****************************************************************************
* SRAM ベリファイテスト
*
******************************************************************************/

void ram_test(void)
{
    B s[40];
    volatile UB *p;
    UW i;
    UB c;

    p = (UB *)0x00430000;

    for (i = 0; i < 0x10000; i++, p++)
    {
        if (i % 1024 == 0)
        {
#ifdef USE_FORMAT   /* sprintf を使用する例 */
            sprintf(s, (char *)"%3ld KB\b\b\b\b\b\b", i / 1024);
            print(s);
#else
            lto_d((char *)s, i / 1024, 3);
            print(s);
            print((B *)" KB\b\b\b\b\b\b");
#endif
        }

        chg_ims(15);
        c = *p;
        *p = 0x55; if (*p != 0x55) goto ERR;
        *p = 0xaa; if (*p != 0xaa) goto ERR;
        *p = 0xff; if (*p != 0xff) goto ERR;
        *p = 0x00; if (*p != 0x00) goto ERR;
        *p = c;
        chg_ims(0);
    }
#ifdef USE_FORMAT   /* sprintf を使用する例 */
    sprintf(s, (char *)"%3ld KB OK", i / 1024);
    puts(s);
#else
    lto_d((char *)s, i / 1024, 3);
    print(s);
    puts((B *)" KB OK");
#endif
    return;

ERR:
    *p = c;
    chg_ims(0);
#ifdef USE_FORMAT   /* sprintf を使用する例 */
    sprintf(s, (char *)"%08X NG", (UW)p - 0x00400000);
#else
    strcpy(lto0X((char*)s, (UW)p - 0x00400000, 8), (char*)" NG");
#endif
    puts(s);
}

/*****************************************************************************
* ROM チェックサムテスト
*
******************************************************************************/

void rom_test(void)
{
    B s[40];
    UB *p;
    UH sum;

    print((B *)"ROM Checksum = ");

    sum = 0;
    p = (UB *)000000L;

    for (;;)
    {
        if ((UW)p % 1024 == 0)
        {
#ifdef USE_FORMAT   /* sprintf を使用する例 */
            sprintf(s, (char *)"%04X\b\b\b\b", sum);
#else
            strcpy(lto0X((char*)s, sum, 4), (char*)"\b\b\b\b");
#endif
            print(s);
            if ((W)p >= 0x20000L)
                break;
        }
        sum += *p++;
    }
    puts((B *)"");
}

/*****************************************************************************
* Flash Memory clear テスト
*
******************************************************************************/

void fls_test(void)
{
    B s[40];
    char* p;
    int n;
    UW i;
    BOOL ok;

    for (n = 0; n < 7; n++)
    {
#ifdef USE_FORMAT   /* sprintf を使用する例 */
        sprintf(s, (char *)"Sector %d = %06XH   ", n, fls_sector[n][0]+FLUSH);
#else
        strcpy((char *)s, (char*)"Sector ");
        p = (char *)s + strlen((char *)s);
        strcpy(ltod(p, n, 1), (char*)" = ");
        p = p + strlen(p);
        strcpy(lto0X(p, fls_sector[n][0]+FLUSH, 6), (char*)"H   ");
#endif
        print(s);
        dly_tsk(100/MSEC);

        ok = flash_clear(fls_sector[n][0], fls_sector[n][1]);
        if (ok)
        {   for (i = fls_sector[n][0]; i < (fls_sector[n][0] + fls_sector[n][1]); i++)
            {   if (*((UB *)(FLUSH+i)) != 0x00)
                    ok = FALSE;
            }
        }
        if (ok)
        {   ok = flash_erase(fls_sector[n][0], fls_sector[n][1]);
            if (ok)
            {   for (i = fls_sector[n][0]; i < (fls_sector[n][0] + fls_sector[n][1]); i++)
                {   if (*((UB *)(FLUSH+i)) != 0xff)
                        ok = FALSE;
                }
            }
            if (ok)
                puts((B *)"... OK");
            else
                puts((B *)"... Erase NG");
        }
        else
            puts((B *)"... Clear NG");
    }
    puts((B *)"");
}

/*****************************************************************************
* Check sum code の計算
*
******************************************************************************/

static UB check_sum(UB *data, int len)
{
    UB sum;

    for (sum = 0; len > 0; len--)
        sum += *data++;
    return sum;
}

/*****************************************************************************
* 各パラメータ入力
*
******************************************************************************/

BOOL read_param(INT n, VP param, INT size)
{
    UB data[16];

    for (;;) {
        if (seq_read_eeprom(n * 32, data, 16)) {
            if (check_sum(data, 15) == data[15])
                break;
        }
        if (seq_read_eeprom(n * 32 + 16, data, 16)) {
            if (check_sum(data, 15) == data[15])
                break;
        }
        return FALSE;
    }
    memcpy(param, data, size);
    return TRUE;
}

BOOL read_default_ipaddr(void)  { return read_param(0, default_ipaddr,  sizeof default_ipaddr); }
BOOL read_ethernet_addr(void)   { return read_param(1, ethernet_addr,   sizeof ethernet_addr); }
BOOL read_subnet_mask(void)     { return read_param(2, subnet_mask,     sizeof subnet_mask); }
BOOL read_default_gateway(void) { return read_param(3, default_gateway, sizeof default_gateway); }
BOOL read_phone_number(void)    { return read_param(4, phone_number,    sizeof phone_number); }
BOOL read_login_name(void)      { return read_param(5, login_name,      sizeof login_name); }
BOOL read_login_password(void)  { return read_param(6, login_password,  sizeof login_password); }

BOOL read_ip_addr(void)
{
    BOOL ok;

    ok  = read_default_ipaddr();
    ok &= read_subnet_mask();
    ok &= read_default_gateway();
    ok &= read_phone_number();
    ok &= read_login_name();
    ok &= read_login_password();
    return ok;
}

/*****************************************************************************
* パラメータの設定
*
******************************************************************************/

void setup_param(INT n, B *prompt, VP param, INT size, B type)
{
    B buf[16], hex[3], ok;
    UB data[16];
    B *p;
    int c, i, len;

    memset(data, 0, sizeof data);
    memcpy(data, param, size);
    ok = FALSE;

    for (;;)
    {
        p = buf;
        if (type == 'I') /* IP address (ddd.ddd.ddd.ddd) */
        {   for (i = 0; i < 4; i++)
            {   itou((char *)p, data[i], itoulen(data[i])); p += strlen((char *)p);
                if (i != 3)
                    *p++ = '.';
            }
        }
        else if (type == 'E') /* Ethernet address (xxxxxx xxxxxx) */
        {   for (i = 0; i < 6; i++)
            {   ito0x((char *)p, data[i], 2); p += 2;
                if (i == 2)
                    *p++ = ' ';
            }
        }
        else /* 'A' ascii */
            strncpy((char *)buf, (char *)data, sizeof buf - 1);
        if (ok)
            print((B *)" --> ");
        else
            print(prompt);
        print(buf);
        if (ok)
            break;

        len = strlen((char *)buf);
        while (len-- > 0)
            print((B *)"\b");
        input(buf, sizeof buf);
        if (buf[0] == '\0')
            break;
        p = buf;
        if (type == 'I') /* IP address (ddd.ddd.ddd.ddd) */
        {   for (i = 0; i < 4; i++)
            {   data[i] = (B)atoi((char *)p);
                if (i != 3)
                {   p = (B *)strchr((char *)p, '.');
                    if (p == NULL)
                        break;
                    p++;
                }
            }
            if (i == 4)
                ok = TRUE;
        }
        else if (type == 'E') /* Ethernet address (xxxxxx xxxxxx) */
        {   for (i = 0; i < 6;)
            {   c = *p++;
                if (c == '\0')
                    break;
                if (!isxdigit(c))
                    continue;
                hex[0] = (B)c;
                c = *p++;
                if (c == '\0')
                   break;
                hex[1] = (B)c;
                hex[2] = '\0';
                data[i++] = (B)xtoi((char *)hex);
            }
            if (i == 6)
                ok = TRUE;
        }
        else /* 'A' ascii */
        {   strcpy((char *)data, (char *)buf);
                ok = TRUE;
        }
        if (!ok)
            puts((B *)"");
    }
    puts((B *)"");

    data[15] = check_sum(data, 15);
    page_write_eeprom(n * 32, data, sizeof data);
    page_write_eeprom(n * 32 + 16, data, sizeof data);
    if (!read_param(n, param, size))
    {   puts((B *)"EEPROM Error!");
        return;
    }
}

void setup_default_ipaddr(void)  { setup_param(0, (B *)"IP Address       :", default_ipaddr,  sizeof default_ipaddr,  'I'); }
void setup_ethernet_addr(void)   { setup_param(1, (B *)"Ethernet Address :", ethernet_addr,   sizeof ethernet_addr,   'E'); }
void setup_subnet_mask(void)     { setup_param(2, (B *)"Subnet Mask      :", subnet_mask,     sizeof subnet_mask,     'I'); }
void setup_default_gateway(void) { setup_param(3, (B *)"Gateway Address  :", default_gateway, sizeof default_gateway, 'I'); }
void setup_phone_number(void)    { setup_param(4, (B *)"Phone Number     :", phone_number,    sizeof phone_number,    'A'); }
void setup_login_name(void)      { setup_param(5, (B *)"Login Name       :", login_name,      sizeof login_name,      'A'); }
void setup_login_password(void)  { setup_param(6, (B *)"Password         :", login_password,  sizeof login_password,  'A'); }

/*****************************************************************************
* Parameter 設定
*
******************************************************************************/

void par_test(void)
{
    B buf[8];
    int c;

    for (;;)
    {
        /* メニュー表示 */

        puts((B *)"[1]Ethernet Addr [2]IP Address   [3]Subnet Mask   [4]Gateway Address");
        puts((B *)"[5]Phone No.     [6]Login Name   [7]Password      [Q]Quit");

        /* キー入力 */
    L1:
        putch('>');
        gets(buf);

        /* 各設定へ */

        c = buf[0];
        switch(c)
        {
        case '\0':
        case '1': setup_ethernet_addr();   if (c) break;
        case '2': setup_default_ipaddr();  if (c) break;
        case '3': setup_subnet_mask();     if (c) break;
        case '4': setup_default_gateway(); if (c) break;
        case '5': setup_phone_number();    if (c) break;
        case '6': setup_login_name();      if (c) break;
        case '7': setup_login_password();  break;
        case 'Q': case 'q': case 0x1b:
            puts((B *)"(note) Please reboot if these parameters are changed.");
            return;
        default:  goto L1;
        }
    }
}

/*****************************************************************************
* CTRL-C シグナルハンドラ
*
******************************************************************************/

static void test_ctrl_c(int sig)
{
    if (sig == SIGINT)
        ctrl_c = TRUE;
}

/*****************************************************************************
* テスト処理のメイン
*
******************************************************************************/

int test_command(int argc, char *argv[])
{
    B buf[32];

    /* タイトル表示 */

    puts((B *)title);

    /* CTRL-C シグナルハンドラの登録 */

    signal(SIGINT, test_ctrl_c);

    /* 以下繰り返し */

    for (;;)
    {
        /* メニュー表示 */

        puts((B *)"[1] LED output test   [2] RTC test     [3] Flash memory test");
        puts((B *)"[4] DIP switch test   [5] SRAM verify  [6] ROM checksum");
        puts((B *)"[7] Parameter set     [8] UDP Echo     [9] TCP Echo   [Q] Quit");

        /* キー入力 */

        putch('>');
        gets(buf);
        while (kbhit())
            getch();

        /* 各テスト実行 */

        ctrl_c = FALSE;

        switch (buf[0])
        {
        case '1':   led_test(); break;
        case '2':   rtc_test(); break;
        case '3':   fls_test(); break;
        case '4':   dip_test(); break;
        case '5':   ram_test(); break;
        case '6':   rom_test(); break;
        case '7':   par_test(); break;
        case '8':   udp_echo_client(0); break;
        case '9':   tcp_echo_client(0); break;
        case 'q':
        case 'Q':   goto END;
        }
    }
END:
    /* CTRL-C シグナルハンドラの登録解除 */

    signal(SIGINT, SIG_DFL);
    return 0;
}

/* end */
