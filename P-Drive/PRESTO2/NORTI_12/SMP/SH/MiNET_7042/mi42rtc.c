/******************************************************************************
* MiNET-7042 RTC-4513 時計入出力関数                                          *
*                                                                             *
*                                                                             *
* 14/Apr/1998 作成                                                            *
* 29/Nov/1998 MiNET-7042 用に修正                                       MiSPO *
* 24/Dec/2002 SH7042/44/45で共通のためsh7042.h->sh7040.hにする             KA *
******************************************************************************/

#include "norti3.h"
#include "sh7040.h"
#include "net7042.h"

/* レジスタアドレス */

#define RTC_S1      0       /* １秒桁レジスタ */
#define RTC_S10     1       /* 10秒桁レジスタ */
#define RTC_M1      2       /* １分桁レジスタ */
#define RTC_M10     3       /* 10分桁レジスタ */
#define RTC_H1      4       /* １時桁レジスタ */
#define RTC_H10     5       /* 10時桁レジスタ */
#define RTC_D1      6       /* １日桁レジスタ */
#define RTC_D10     7       /* 10日桁レジスタ */
#define RTC_MO1     8       /* １月桁レジスタ */
#define RTC_MO10    9       /* 10月桁レジスタ */
#define RTC_Y1      10      /* １年桁レジスタ */
#define RTC_Y10     11      /* 10年桁レジスタ */
#define RTC_W       12      /* 曜日桁レジスタ */
#define RTC_CD      13      /* コントロールレジスタＤ */
#define RTC_CE      14      /* コントロールレジスタＥ */
#define RTC_CF      15      /* コントロールレジスタＦ */

/* フラグ */

#define RTC_FO  0x08        /* OSC 発振停止 */
#define RTC_FR  0x08        /* 読み出し中桁上げ有り */

/* ＣD レジスタのビット */

#define RTC_HOLD    0x01
#define RTC_CAL     0x02
#define RTC_IRQ_F   0x04
#define RTC_ADJ30   0x08

/* ＣE レジスタのビット */

#define RTC_MASK    0x01
#define RTC_INT     0x02
#define RTC_T0      0x04
#define RTC_T1      0x08

/* ＣF レジスタのビット */

#define RTC_RESET   0x01
#define RTC_STOP    0x02
#define RTC_12      0x00
#define RTC_24      0x04
#define RTC_TEST    0x08

/* RTC I/O のビット */

#define RTC_CLK_bit     0x0040
#define RTC_DATA_bit    0x0010
#define RTC_CE_bit      0x0080

/* 入出力マクロ */

#define RTC_CLK_ON()    sfr_setw(IO_PEDR,RTC_CLK_bit)
#define RTC_CLK_OFF()   sfr_clrw(IO_PEDR,RTC_CLK_bit)
#define RTC_DATA_RD()   sfr_clrw(PFC_PEIOR,RTC_DATA_bit)
#define RTC_DATA_WR()   sfr_setw(PFC_PEIOR,RTC_DATA_bit)
#define RTC_CE_ON()     sfr_setw(IO_PEDR,RTC_CE_bit)
#define RTC_CE_OFF()    sfr_clrw(IO_PEDR,RTC_CE_bit)
#define RTC_DATA_1()    sfr_setw(IO_PEDR,RTC_DATA_bit)
#define RTC_DATA_0()    sfr_clrw(IO_PEDR,RTC_DATA_bit)
#define RTC_DATA_IN()   ((sfr_inw(IO_PEDR)&RTC_DATA_bit)!=0)

/*****************************************************************************
* BCD -> 2進数
*
******************************************************************************/

static int bcd2bin(UB c)
{
    return (((c >> 4) & 0x0f) * 10) + (c & 0x0f);
}

/*****************************************************************************
* 2進数 -> BCD
*
******************************************************************************/

static UB bin2bcd(int c)
{
    if (c == -1)
        return 0xff;
    return (UB)(((c / 10) << 4) | (c % 10));
}

/*****************************************************************************
* ちょい待ち
*
******************************************************************************/

static int wait(int i)
{
    while (--i != 0)
        ;
    return i;
}

/*****************************************************************************
* RTC-4513 複数レジスタ読み出し処理
*
******************************************************************************/

static void read_rtc(UB addr, int cnt, UB *data)
{
    int i, j;
    UB c;
    UINT psw;

    psw = vdis_psw();
    RTC_CLK_OFF();
    RTC_CE_ON();
    RTC_DATA_WR();

    /* アドレス＋ 読み出しコード 0x0c */

    addr = (UB)((addr << 4) | 0x0c);

    for (i = 0; i < 8; i++)
    {
        if (addr & 0x01)
            RTC_DATA_1();
        else
            RTC_DATA_0();
        RTC_CLK_ON();
        addr >>= 1;
        RTC_CLK_OFF();
    }

    /* データ読み出し */

    RTC_DATA_RD();

    for (j = 0; j < cnt; j++)
    {
        c = 0;

        for (i = 0; i < 4; i++)
        {
            RTC_CLK_ON();
            c >>= 1;
            if (RTC_DATA_IN())
                c |= 0x08;
            RTC_CLK_OFF();
        }

        *data++ = c;
    }

    RTC_CE_OFF();
    RTC_CLK_ON();
    RTC_CLK_OFF();
    vset_psw(psw);
}

/*****************************************************************************
* RTC-4513 複数レジスタ書き込み処理
*
******************************************************************************/

static void write_rtc(UB addr, int cnt, UB *data)
{
    int i, j;
    UB c;
    UINT psw;

    psw = vdis_psw();
    RTC_CLK_OFF();
    RTC_CE_ON();
    RTC_DATA_WR();

    /* アドレス＋ 書き込みコード 0x03 */

    addr = (UB)((addr << 4) | 0x03);

    for (i = 0; i < 8; i++)
    {
        if (addr & 0x01)
            RTC_DATA_1();
        else
            RTC_DATA_0();

        RTC_CLK_ON();
        addr >>= 1;
        RTC_CLK_OFF();
    }

    /* データ書き込み */

    for (j = 0; j < cnt; j++)
    {
        c = *data++;

        for (i = 0; i < 4; i++)
        {
            if (c & 0x01)
                RTC_DATA_1();
            else
                RTC_DATA_0();
            RTC_CLK_ON();
            c >>= 1;
            RTC_CLK_OFF();
        }
    }

    RTC_CE_OFF();
    RTC_DATA_RD();
    RTC_CLK_ON();
    RTC_CLK_OFF();
    vset_psw(psw);
}

/*****************************************************************************
* 時計読み出し
*
* 引数  time    日付構造体へのポインタ
*
* 戻値  TRUE:成功, FALSE:失敗
******************************************************************************/

BOOL gettime(struct tm *time)
{
    UB d[13];
    int i;

    for (i = 10; i != 0; i--)
    {
        read_rtc(RTC_S1, 13, d);    /* S1～W */

        /* 読み出し中に桁上げあり？ */

        if (!(d[12] & RTC_FR))
            break;
    }

    time->tm_sec  = bcd2bin((UB)((d[1]  << 4) | d[0]));
    time->tm_min  = bcd2bin((UB)((d[3]  << 4) | d[2]));
    time->tm_hour = bcd2bin((UB)((d[5]  << 4) | d[4]));
    time->tm_mday = bcd2bin((UB)((d[7]  << 4) | d[6]));
    time->tm_mon  = bcd2bin((UB)((d[9]  << 4) | d[8])) - 1;
    time->tm_year = bcd2bin((UB)((d[11] << 4) | d[10]));
    time->tm_wday = bcd2bin(d[12]);
    time->tm_yday = 0;                  /* 年初からの日数は未サポート */
    time->tm_isdst = 0;                 /* 夏時間は未サポート */

    if (i == 0)
        return FALSE;
    else
        return TRUE;
}

/*****************************************************************************
* 時計設定
*
* 設定にあたって tm_yday と tm_isdst は使用していません。
******************************************************************************/

BOOL settime(struct tm *time)
{
    UB d[13];

    if (time->tm_year < 0 || time->tm_year > 99)
        return FALSE;
    if (time->tm_mon < 0 || time->tm_mon > 11)
        return FALSE;
    if (time->tm_mday < 1 || time->tm_mday > 31)
        return FALSE;
    if (time->tm_hour < 0 || time->tm_hour > 23)
        return FALSE;
    if (time->tm_min < 0 || time->tm_min > 59)
        return FALSE;
    if (time->tm_sec < 0 || time->tm_sec > 59)
        return FALSE;
    if (time->tm_wday < 0 || time->tm_wday > 6)
        return FALSE;

    /* 計時停止 */

    d[0] = (UB)(RTC_RESET|RTC_STOP|RTC_24);
    write_rtc(RTC_CF, 1, d);

    /* 2進数 -> BCD 変換 */

    d[0]  = bin2bcd(time->tm_sec);      /* 秒 */
    d[1]  = (UB)((d[0] >> 4) & 0x0f);
    d[2]  = bin2bcd(time->tm_min);      /* 分 */
    d[3]  = (UB)((d[2] >> 4) & 0x0f);
    d[4]  = bin2bcd(time->tm_hour);     /* 時 */
    d[5]  = (UB)((d[4] >> 4) & 0x0f);
    d[6]  = bin2bcd(time->tm_mday);     /* 日 */
    d[7]  = (UB)((d[6] >> 4) & 0x0f);
    d[8]  = bin2bcd(time->tm_mon + 1);  /* 月 */
    d[9]  = (UB)((d[8] >> 4) & 0x0f);
    d[10] = bin2bcd(time->tm_year);     /* 年 */
    d[11] = (UB)((d[10] >> 4) & 0x0f);
    d[12] = bin2bcd(time->tm_wday);     /* 曜 */

    /* S1～W 書き込み */

    write_rtc(RTC_S1, 13, d);

    /* 計時開始 */

    d[0] = RTC_24;
    write_rtc(RTC_CF, 1, d);

    /* 電源投入時はこの設定も必要 */

    d[0] = (UB)(RTC_T1|RTC_T0);
    write_rtc(RTC_CE, 1, d);
    d[0] = RTC_CAL;
    write_rtc(RTC_CD, 1, d);

    return TRUE;
}

/* end */
