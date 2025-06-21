/******************************************************************************
* NORTi3 Network PHY I/F Control (DP83843/National Semiconductor)             *
*                                                                             *
*  File name : dp83843.c                                                      *
*  Copyright (c) 2000-2001 MiSPO Co., Ltd.  All rights reserved.              *
*                                                                             *
* 13/Apr/2000 作成                                                      MiSPO *
* 15/Feb/2001 Auto Negotiation 完了を待つようにする                  OK(MiSPO)*
* 25/Apr/2001 SH7615 EVA BOD+ 用設定変更(PHY_PHYCTRL)                OK       *
*             Auto Negotiation 以外の設定時もLINK確立待ちするように           *
* 30/Oct/2002 GAIO(XASSV)対応(delay()の追加)                               KA *
******************************************************************************/

/*

PHY(DP83843)ディバイスアドレスの設定

          MIIのディバイス局アドレスのデフォルトは、0x01 です。
          これを変更する場合、PHYAD マクロを定義してコンパイルしてください。

          (例) shc <option> -def=PHYAD=0x03 dp83843.c
                            ~~~~~~~~~~~~~~~
          (例) gcc <option> -DPHYAD=0x03 dp83843.c
                            ~~~~~~~~~~~~
          (例) ccsh <options> -DPHYAD=0x03 dp83843.c
                              ~~~~~~~~~~~~

強制ループバックの設定

          PHY(DP83843)内部 折り返しの設定を有効にする場合には
          LOOP_BACK マクロを定義してコンパイルしてください。

          (例) shc <option> -def=LOOP_BACK dp83843.c
                            ~~~~~~~~~~~~~~
          (例) gcc <option> -DLOOP_BACK dp83843.c
                            ~~~~~~~~~~~
          (例) ccsh <options> -DLOOP_BACK dp83843.c
                              ~~~~~~~~~~~

*/

#include "norti3.h"
#include "sh7615.h"
#include "sh7615e.h"
#include "dp83843.h"

#ifndef PHYAD
#define PHYAD 0x01
#endif

static const UB PHY_ad = PHYAD;    /* PHY device addres */

/*****************************************************************************
* PHYディバイスアドレス取得
*
******************************************************************************/

UB PHY_get_adr(void)
{
    return PHY_ad;
}

#if defined(__GNUC__)     /* exeGCC */
/*****************************************************************************
* MII デバイス(DP83843)アクセスウェイト
*
*  delay time = 5 * (count - 1) + 7 tcycle(system clock)
*      count  = 0(Max.) : about 357.9[sec]   @60.0MHz
*      count  = 1(Min.) : about 116.6[nsec]  @60.0MHz
*
* 戻り値は不定です
*
******************************************************************************/

static UW delay(UW count)
{
 asm("
    mov     #0,r0
                        !r4 = count
1:                      ! delay time = (5 * (count-1)) + 3
                        !                   + 2(function call time)
                        !                   + 2(return time)
                        !                   + α

    add     #-1,r4      !--count            1tcyc
    cmp/eq  r0,r4       !if count != 0      1tcyc
    bf      1b          !                   3/1tcyc
 ");
}

#elif defined(__ghs)   /* Green Hills */
/*****************************************************************************
* MII デバイス(DP83843)アクセスウェイト
*
*  delay time = 5 * (count - 1) + 7 tcycle(system clock)
*      count  = 0(Max.) : about 357.9[sec]   @60.0MHz
*      count  = 1(Min.) : about 116.6[nsec]  @60.0MHz
*
* 戻り値は不定です
*
******************************************************************************/

static UW delay(UW count)
{
    asm("   mov     #0,r0   ");
    asm("1:                 ");
    asm("   add     #-1,r4  ");
    asm("   cmp/eq  r0,r4   ");
    asm("   bf      1b      ");
}

#elif defined(GAIO)     /* XASSV */
/*****************************************************************************
* MII デバイス(DP83843)アクセスウェイト
*
*  delay time = 5 * (count - 1) + 7 tcycle(system clock)
*      count  = 0(Max.) : about 357.9[sec]   @60.0MHz
*      count  = 1(Min.) : about 116.6[nsec]  @60.0MHz
*
* 戻り値は不定です
*
******************************************************************************/

static UW delay(UW count)
{
#asm
    mov     #0,r0
L1:
    add     #-1,r4
    cmp/eq  r0,r4
    bf      L1
#endasm
}

#else                     /* Renesas C */
/*****************************************************************************
* MII デバイス(DP83843)アクセスウェイト
*
*  delay time = 5 * (count - 1) + 7 tcycle(system clock)
*      count  = 0(Max.) : about 357.9[sec]   @60.0MHz
*      count  = 1(Min.) : about 116.6[nsec]  @60.0MHz
*
* 戻り値は不定です
*
******************************************************************************/

#pragma inline_asm(_delay)
static UW _delay(UW count)
{
    mov     #0,r0
                        ;r4 = count
?__delayS:              ; delay time = (5 * (count-1)) + 3
                        ;                   + 2(function call time)
                        ;                   + 2(return time)
                        ;                   + α
;
    add     #-1,r4      ;--count            1tcyc
    cmp/eq  r0,r4       ;if count != 0      1tcyc
    bf      ?__delayS   ;                   3/1tcyc
}

static UW delay(UW count)
{
    return(_delay(count));  /* 関数の実態は、こっちです。 */
}
#endif

/*****************************************************************************
* MII 1ビット書き込み
*
******************************************************************************/

static void MII_snd_1(BOOL dat)
{
    sfr_outl(ETHERC_PIR,((dat << 2)&PIR_MDO)|PIR_MMD); /* MMD=1,MDO=dat,MDC=0 */
    sfr_setl(ETHERC_PIR,PIR_MDC);                      /* MMD=1,MDO=dat,MDC=1 */
    delay(3);  /* wait for CLK high(272nsec) */
    sfr_clrl(ETHERC_PIR,PIR_MDC);                      /* MMD=1,MDO=dat,MDC=0 */
    delay(2);  /* wait for CLK low(192nsec) */
}

/*****************************************************************************
* MII 1ビット読み込み
*
******************************************************************************/

static BOOL MII_rcv_1(void)
{
    BOOL dat;

    sfr_outl(ETHERC_PIR,PIR_MDC);       /* MMD=0,MDC=1 */
    delay(4);  /* wait for PHY out delay time(352nsec) */
    dat = (sfr_inl(ETHERC_PIR)>>3) & 1;     /* Get MDI */
    sfr_clrl(ETHERC_PIR,PIR_MDC);       /* MMD=0,MDC=0 */
    delay(1);  /* wait for CLK low(116nsec) */
    return (dat);
}

/*****************************************************************************
* MII バス解放
*
******************************************************************************/

static void MII_hiZ(void)
{
    sfr_outl(ETHERC_PIR,0);       /* MMD=0,MDC=0 */
    sfr_setl(ETHERC_PIR,PIR_MDC); /* MMD=0,MDC=1 */
    delay(3);  /* wait for CLK high(272nsec) */
    sfr_clrl(ETHERC_PIR,PIR_MDC); /* MMD=0,MDC=0 */
    delay(2);  /* wait for CLK low(192nsec) */
}

/*****************************************************************************
* MII 単独バス解放(IDLE)
*
******************************************************************************/

static void MII_idle(void)
{
    sfr_outl(ETHERC_PIR,0x0);  /* MMD=0,MDC=0 */
}

/*****************************************************************************
* MII 16ビット書き込み
*
******************************************************************************/

static void MII_snd_16(UH dat)
{
    int i;

    for ( i=0; i<16; i++, dat<<=1 )
        MII_snd_1( (dat & 0x8000)?1:0 );
}

/*****************************************************************************
* MII 16ビット読み込み
*
******************************************************************************/

static UH MII_rcv_16(void)
{
    UH dat = 0;
    int i;

    for ( i=0; i<16; i++ ){
        dat <<= 1;
        dat |= ((UH)MII_rcv_1() & 1);
    }
    return (dat);
}

/*****************************************************************************
* PHY レジスタへの書き込み
*
* WRITE FORMAT
* <preamble><start><op code><device addr><reg addr><turnaround><data>   <idle>
* 1..1/32bits  01      01        ?????      ?????       10   D..D/16bits Z..Z
*
*   ?: 任意bits / HW依存
*   Z: High-Z
*   D: data bits
*
******************************************************************************/

void PHY_write(UB adr, UB reg, UH dat)
{
    int i;

    MII_snd_16(0xffff);                /* PREAMBLE */
    MII_snd_16(0xffff);                /* PREAMBLE */
    MII_snd_1(0);                      /* ST       */
    MII_snd_1(1);                      /* ST       */
    MII_snd_1(0);                      /* OP       */
    MII_snd_1(1);                      /* OP       */
    for ( i=0; i<5; i++, adr<<=1 )
        MII_snd_1( (adr & 0x10)?1:0 ); /* PHYAD    */
    for ( i=0; i<5; i++, reg<<=1 )
        MII_snd_1( (reg & 0x10)?1:0 ); /* REGAD    */
    MII_snd_1(1);                      /* TA       */
    MII_snd_1(0);                      /* TA       */
    MII_snd_16(dat);                   /* DATA     */
    MII_idle();                        /* BUS release */
}

/*****************************************************************************
* PHY からレジスタを読み込む
*
* READ FORMAT
* <preamble><start><op code><device addr><reg addr><turnaround><data>   <idle>
* 1..1/32bits  01      10        ?????      ?????      (Z0)  D..D/16bits Z..Z
*
*   ?: 任意bits / HW依存
*   Z: High-Z
*   D: data bits
*
******************************************************************************/

UH PHY_read(UB adr, UB reg)
{
    int i;

    MII_snd_16(0xffff);                /* PREAMBLE */
    MII_snd_16(0xffff);                /* PREAMBLE */
    MII_snd_1(0);                      /* ST       */
    MII_snd_1(1);                      /* ST       */
    MII_snd_1(1);                      /* OP       */
    MII_snd_1(0);                      /* OP       */
    for ( i=0; i<5; i++, adr<<=1 )
        MII_snd_1( (adr & 0x10)?1:0 ); /* PHYAD    */
    for ( i=0; i<5; i++, reg<<=1 )
        MII_snd_1( (reg & 0x10)?1:0 ); /* REGAD    */
    MII_hiZ();                         /* TA       */
    return (MII_rcv_16());             /* DATA     */
}

/*****************************************************************************
* PHY のSoftリセット
*
******************************************************************************/

static void PHY_reset(void)
{
    PHY_write(PHY_ad, PHY_BMCR, PHY_BMCR_RESET);
}

/*****************************************************************************
* PHY(DP83843) の初期設定
*
******************************************************************************/

static void PHY_setup(void)
{
    PHY_write(PHY_ad, PHY_ANNPTR, 0x3801);
    PHY_write(PHY_ad, PHY_MIPSCR, 0x0002);
    PHY_write(PHY_ad, PHY_DCR,    0x0000);
    PHY_write(PHY_ad, PHY_FCSCR,  0x0000);
    PHY_write(PHY_ad, PHY_RECR,   0x0000);
    PHY_write(PHY_ad, PHY_PCSR,   0x0000);
    PHY_write(PHY_ad, PHY_LBR,    0x0000);
    PHY_write(PHY_ad, PHY_BTSCR,  0x0080);
#if defined(SH7615EV)
    PHY_write(PHY_ad, PHY_PHYCTRL,0x0000+PHY_ad);
#else
    PHY_write(PHY_ad, PHY_PHYCTRL,0x0800+PHY_ad);
#endif
}

/*****************************************************************************
* Auto Negotiation モードにする
*
******************************************************************************/

static void PHY_auto_nego(UH mode_set)
{
    int i;

    PHY_write(PHY_ad, PHY_ANAR, mode_set|0x01);
#ifndef LOOP_BACK
    PHY_write(PHY_ad, PHY_BMCR, PHY_BMCR_ANE|PHY_BMCR_REANE);
    for (i=0; i<15; i++){     /* wait for complete(MAX.1500msec) */
        dly_tsk(100/MSEC);
        if (!(PHY_read(PHY_ad, PHY_BMCR) & PHY_BMCR_REANE))
            break;
    }
#else
    /* 強制ループバックの設定 */
    PHY_write(PHY_ad, PHY_BMCR, PHY_BMCR_ANE|PHY_BMCR_REANE|PHY_BMCR_LOOP);
#endif
}

/*****************************************************************************
* PHY 初期化
*
******************************************************************************/

void PHY_init(BOOL auto_nego_able, UH mode)
{
    int i;

    PHY_reset();              /* soft reset PHY(DP83843) */
    for (i=0; i<50; i++){     /* wait for reset complete(MAX.500msec) */
        if (!(PHY_read(PHY_ad, PHY_BMCR) & PHY_BMCR_RESET))
            break;
        dly_tsk(10/MSEC);
    }
    PHY_setup();              /* setup PHY(DP83843) */

    /* set mode */
    if ( auto_nego_able == TRUE ){
        PHY_auto_nego(mode);
    }
    else{
#ifndef LOOP_BACK
        PHY_write(PHY_ad, PHY_BMCR, mode);
#else
    /* 強制ループバックの設定 */
        PHY_write(PHY_ad, PHY_BMCR, mode|PHY_BMCR_LOOP);
#endif
    }
    for (i=0; i<30; i++){     /* wait for complete(MAX.3000msec) */
        if (PHY_read(PHY_ad, PHY_PHYSTS) & PHY_PHYSTS_LINK)
            break;
        dly_tsk(100/MSEC);
    }
}

/* end */
