/****************************************************************************
* インターバル割込みハンドラ（SH7055）                                      *
*                                                                           *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -op=1 -def=CLK=?,CH=? n4i7055.c                  (SHC)     *
*                                                                           *
* 06/Feb/2001                                                               *
* 27/Mar/2001  GAIO/XASSV 対応                                           TA *
* 12/Sep/2001 制御系の判断を見直し                                       TA *
* 23/Dec/2002 INTVSZ=0対応漏れ修正, inthdrを割込みベクタ(ROM)に記述可    KA *
* 29/Jul/2003 ベクタROM定義時の見直し                                    KA *
****************************************************************************/

/*

最適化コンパイル

          割り込みハンドラのコンパイルには、必ず、最適化オプションを付け、
          ent_int 呼び出しの前に、不要な命令が生成されないよう注意してくださ
          い。
          SHCでは、割込みハンドラを #pragma interrupt と定義してください。

使用するハードウェア

          SH7055 内蔵 ATU-II のチャネル 3～7をインターバル･タイマとして使用
          しています。
          それ以外のタイマを用いる場合は、ユーザーで改造してください。

割込み周期の指定

          割込み周期は、MSEC マクロで決まります。デフォルトは 10 msec です。
          例えば、5 msec に変更する場合は、次の様にコンパイルしてください。

          (例) shc -op=1 -def=MSEC=5 n4i7055.c
                              ~~~~~~
クロックの指定

          内部クロックのデフォルトは 40 MHz です。変更する場合 は CLK マクロ
          を定義してコンパイルしてください。値は [Hz] 単位の周波数です。

          (例) shc -op=1 -def=CLK=19660800 n4i7055.c ･･･ 19.6608 MHz
                              ~~~~~~~~~~~~
チャネル番号の指定

          CH マクロで、使用する ITU チャネルを指定できます。（CH=3～7）
          デフォルトは CH=3 です。

          (例) shc -op=1 -def=CH=6 n4i7055.c  ･･･････ ATU ﾁｬﾈﾙ6
                              ~~~~
割込みベクタテーブル

          def_inh システムコールによる割込みベクタの設定を可能にするため、
          RAM 先頭（9000000H 番地から 1024 バイト）に、割込みベクタテーブルが
          あると想定しています。
          割込みベクタテーブルの領域が、他の領域と重ならない様にリンクしてく
          ださい。（重なってもリンカで警告は出ません!）
          割込みベクタテーブル先頭アドレスは INTVEC マクロで、サイズ（バイト
          数）は、INTVSZ マクロで変更できます。

          (例) shc -op=1 -def=INTVEC=0,INTVSZ=464 n4i7055.c ･･･ 0番地,464ﾊﾞｲﾄ
                              ~~~~~~~~ ~~~~~~~~~~
          割込みベクタテーブルが ROM 上にあり、def_inh に依らず直接ベクタを定
          義する場合、INTVSZ=0 としてください。
          この場合、def_inh システムコールは機能しません。

割込み優先順位

          本ハンドラの、デフォルトの割込み優先順位は、7 です。
          これを変更する場合、IP マクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。（IP ≦ KNL_LEVEL）
          本ハンドラより高優先度の割込みハンドラが有る場合、多重割込みが起こ
          ります。

          (例) shc -op=1 -def=IP=2 n4i7055.c ･･･ 割込み優先順位 2
                              ~~~~
dis_int, ena_int

          割り込みレベル設定のビット配置が不規則な SH7055 の割込みコントロー
          ラでは、汎用的な dis_int, ena_int システムコールの実装が複雑となっ
          てしまいます。
          この２つのシステムコールは用意していませんので、個別の割込み禁止許
          可を行いたい場合は、各制御レジスタを直接アクセスしてください。
*/

#ifndef SH2E
#define SH2E
#endif

#include "kernel.h"
#include "nosys4.h"
#include "sh7055.h"

/* 割込みベクタテーブル */

#ifndef INTVEC
#define INTVEC      0x0400000L
#endif
#ifndef INTVSZ
#define INTVSZ      1024
#endif

/* 割込み優先順位の定義 */

#ifndef IP
#define IP      7               /* IP 未定義なら 7 とする */
#endif

/* クロックの定義 */

#define MHz         1000000L
#ifndef CLK
#define CLK         40*MHz      /* CPUクロック[Hz] */
#endif

/* チャネル番号の定義 */

#ifndef CH
#define CH      3               /* CH 未定義なら 3 とする */
#endif

/* 処理系の差異の調整 */

#if defined(__GNUC__)           /* exeGCC */
void disable(void);
void enable(void);
void set_cr(int cr);
int get_cr(void);
void set_vbr(void **base);
void **get_vbr(void);
extern char _srdata[], _erdata[];
extern char _stext[], _etext[];

#elif defined(__ghs)            /* Green Hills */
void disable(void);
void enable(void);
void set_cr(int cr);
int get_cr(void);
void set_vbr(void **base);
void **get_vbr(void);
extern char __ghsbegin_rodata[], __ghsend_rodata[];
extern char __ghsbegin_text[], __ghsend_text[];

#elif defined(__TID__)           /* IAR EW/ICC */
void set_vbr(void **base);
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;

#elif defined(GAIO)             /* XASSV */
#include <ht1_inline.h>
extern UW CONST_TOP[], CONST_SIZE[];

#elif defined(__HITACHI__)      /* Renesas C */
#include <machine.h>
#define disable()   set_cr(15 << 4);
#define enable()    set_cr(0)
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;
#if (INTVSZ==0)
#pragma interrupt(inthdr)
#endif
#endif

extern int RST_VECT;

#ifdef HOOK
/* チック入力積算カウンタ */

UW TOTAL_TICKCK;
UW TICK_RATE;
#endif

extern  UB _KERNEL_pisrposi;
extern  UB _KERNEL_hdrlen;
extern  void _kernel_interr(void);


/*****************************************************************************
* 割込み初期化
*
* 形式  intini();
*
* 解説  カーネルの初期化ルーチン（sysini）から割込み禁止状態で呼び出されます。
******************************************************************************/

ER intini(void)
{   /* 割込みテーブルをROM上に定義した場合は INTVSZ == 0 */
  #if (INTVSZ!=0)

    INT i;
    static int *p;

    /* 割込みベクタテーブルの初期化 */

    for (i = INT_IRQ0; i < (INTVSZ) / 4; i++)
        ((UW *)INTVEC)[i] = (UW)_kernel_interr;

    /* ベクタベースレジスタを設定 */
    /* リモートデバッガ(リセットベクタが0番地でない場合)は除く */

    p = &RST_VECT;
    if (p == (int *)0L)
        set_vbr((void **)INTVEC);

  #endif
  #ifdef HOOK
    TOTAL_TICKCK = 0;
  #endif

    return E_OK;
}

/*****************************************************************************
* 定数か否かをチェックする
*
* 形式  result = _kernel_inROM(const void *p);
*
*       VP p;         チェックする領域へのポインタ
*       BOOL result;  TRUE .... 定数 or コードセクション
*                     FALSE ... 上記以外
*
* 解説  パケット生成情報等が定数か変数かを判定します。
*       カーネルの各種システムコールから呼び出されます。
******************************************************************************/

BOOL _kernel_inROM(const void *p)
{
#if defined(__GNUC__)           /* exeGCC */
    return((((UW)p >= (UW)_stext) && ((UW)p <= (UW)_etext)) || (((UW)p >= (UW)_srdata) && ((UW)p <= (UW)_erdata)));

#elif defined(__ghs)            /* Green Hills */
    return((((UW)p >= (UW)__ghsbegin_text) && ((UW)p <= (UW)__ghsend_text)) || (((UW)p >= (UW)__ghsbegin_rodata) && ((UW)p <= (UW)__ghsend_rodata)));

#elif defined(GAIO)             /* XASSV */
    return(((UW)p >= (UW)CONST_TOP) && ((UW)p <= ((UW)CONST_TOP+(UW)CONST_SIZE)));

#elif defined(__HITACHI__)      /* Renesas C */
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));

#elif defined(__TID__)           /* IAR EW/ICC */
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));

#endif
}

/*****************************************************************************
* 割込みサービスルーチン定義情報のチェック
*
* 形式  ercd = _kernel_check_cisr(const T_CISR *pk_cisr);
*
*       T_CISR pk_cisr;  チェックする割込みサービスルーチン定義情報へのポインタ
*       ER ercd;         E_PAR ... 割込み番号とマスクレベルのいずれかが範囲外
*                        E_OK .... 割込み番号とマスクレベルは共に正常
*
* 解説  割込みサービスルーチン定義情報の割込み番号とマスクレベルの範囲を
*       チェックします。
*       cre_isr(), acre_isr() システムコールから呼び出されます。
******************************************************************************/

ER _kernel_check_cisr(const T_CISR *pk_cisr)
{
    if (pk_cisr->intno >= (INTVSZ) / 4)
        return E_PAR;
    return E_OK;
}

/*****************************************************************************
* 割込みハンドラの書き込み処理
*
* 形式  _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr);
*
*       FP hdr;          割込みサービスルーチン使用時の割込みハンドラアドレス
*       T_CISR pk_cisr;  チェックする割込みサービスルーチン定義情報へのポインタ
*
* 解説  割込みサービスルーチン使用時の割込みハンドラのアドレスを割込みサービス
*       ルーチン定義情報の割込み番号より算出したアドレスに格納します。
*       cre_isr(), acre_isr() システムコールから呼び出されます。
******************************************************************************/

void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    *p = (UW)hdr;
}

/*****************************************************************************
* 割込みハンドラの読み出し処理
*
* 形式  inthdr = _kernel_get_inthdr(const T_CISR *pk_cisr);
*
*       T_CISR pk_cisr;  チェックする割込みサービスルーチン定義情報へのポインタ
*       FP inthdr;       当該割込み番号の割込みハンドラアドレス
*
* 解説  割込みサービスルーチン定義情報の割込み番号より算出したアドレスから
*       割込みハンドラアドレスを読み出します。
*       cre_isr(), acre_isr() システムコールから呼び出されます。
******************************************************************************/

FP _kernel_get_inthdr(const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    return (FP)*p;
}

/*****************************************************************************
* 割込みハンドラを定義する
*
* 形式  ercd = def_inh(UINT inhno, const T_DINH *pk_dinh);
*
*       UINT inhno; 　    割込みベクタ番号
*       T_DINH *pk_dinh; 割込みハンドラ定義情報
*         ATR intatr;    割込みハンドラ属性（TA_HLNG のみ, 未参照）
*         FP inthdr;     割込みハンドラアドレス
*         UINT imask;    割込みマスクレベル（未使用）
*       ER ercd;         エラーコード
*
* 解説  inhno に対応する割込みベクタテーブルに、inthdr を設定します。
******************************************************************************/

ER v4_def_inh(INHNO inhno, const V4_T_DINH *pk_dinh)
{
  #if (INTVSZ==0)

    return E_NOSPT;

  #else

    UINT psw;
    UINT i;
    UW *p;
    FP fp;

    /* 割込みベクタテーブルへのポインタ算出 */

    i = inhno;
    if (i >= (INTVSZ) / 4)
        return E_PAR;
    p = (UW *)INTVEC + i;

    /* 定義解除の場合 */
    /* 未定義割込みを設定 */

    if (pk_dinh == (T_DINH *)NADR)
        fp = (FP)_kernel_interr;
    else
        fp = pk_dinh->inthdr;

    /* CPU割込み禁止 */

    psw = vdis_psw();

    /* 定義の場合 */
    /* 新しい割込みベクタ設定 */

    *p = (UW)fp;

    /* CPU割込み禁止戻す */
    vset_psw(psw);
    return E_OK;

  #endif
}


/*****************************************************************************
* インターバルタイマ起動（SH7055 内蔵 ATU）
*
* 割込み禁止状態で実行してください。
******************************************************************************/

#if (CH==3)
#define ATU_TCR     ATU_TCR3
#define ATU_IMFD_b  0x0008
#define ATU_TSTR_b  0x10
#define ATU_TCNT    ATU_TCNT3
#define ATU_TIOR    ATU_TIOR3B
#define ATU_GRD     ATU_GR3D
#define ATU_TMDR_b  0x01
#define INT_IMID    INT_IMI3D
#elif (CH==4)
#define ATU_TCR     ATU_TCR4
#define ATU_IMFD_b  0x0100
#define ATU_TSTR_b  0x20
#define ATU_TCNT    ATU_TCNT4
#define ATU_TIOR    ATU_TIOR4B
#define ATU_GRD     ATU_GR4D
#define ATU_TMDR_b  0x02
#define INT_IMID    INT_IMI4D
#elif (CH==5)
#define ATU_TCR     ATU_TCR5
#define ATU_IMFD_b  0x2000
#define ATU_TSTR_b  0x40
#define ATU_TCNT    ATU_TCNT5
#define ATU_TIOR    ATU_TIOR5B
#define ATU_GRD     ATU_GR5D
#define ATU_TMDR_b  0x04
#define INT_IMID    INT_IMI5D
#elif (CH==6)
#define ATU_TCR     ATU_TCR6B
#define ATU_TSR     ATU_TSR6
#define ATU_TIER    ATU_TIER6
#define ATU_PSCR    ATU_PSCR2
#define ATU_CYLRD   ATU_CYLR6D
#define ATU_TCNT    ATU_TCNT6D
#define INT_CMID    INT_CMI6D
#define ATU_TSTR_b  0x08
#elif (CH==7)
#define ATU_TCR     ATU_TCR7B
#define ATU_TSR     ATU_TSR7
#define ATU_TIER    ATU_TIER7
#define ATU_PSCR    ATU_PSCR3
#define ATU_CYLRD   ATU_CYLR7D
#define ATU_TCNT    ATU_TCNT7D
#define INT_CMID    INT_CMI7D
#define ATU_TSTR_b  0x80
#else
#error illegal CH number! (CH = 3 to 7)
#endif

#if ((CH>=3)&&(CH<=5))

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_IMID, isritm, 0 };
#endif

ER intsta(void)
{
    static UB const prescale[] = { 1, 2, 4, 8, 16, 32 };
    UW clk,tc;
    INT i;
    UB tpsc;

    /* 時定数を計算 */

    clk /= sfr_in(ATU_PSCR1)+1;
    for (i = 0; i < sizeof(prescale); i++)
    {   tc = ((UW)(CLK) * MSEC)/((UW)prescale[i] * 1000 * 2);
        if (tc <= 0x10000L)
            break;
    }
    if (i == sizeof(prescale) || tc == 0L)
        return E_SYS;
    tpsc = i;

    /* 割込み定義 */

  #if (INTVSZ!=0)
    acre_isr(&disr_isritm);
  #endif

    /* 割込み優先レベル設定 */

  #if (CH==3)
    sfr_outw(INTC_IPRF, sfr_inw(INTC_IPRF) | (IP << 12));
  #elif (CH==4)
    sfr_outw(INTC_IPRF, sfr_inw(INTC_IPRF) | (IP <<  4));
  #elif (CH==5)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | (IP << 12));
  #endif

    /* タイマユニット初期化 */

    sfr_outw(ATU_GRD, (H)(tc-1));       /* 比較値 */
    sfr_out(ATU_TCR, tpsc);             /* クロック選択 */
    sfr_setw(ATU_TIER3, ATU_IMFD_b);    /* IMID 割込み許可 */
    sfr_outw(ATU_TCNT, 0x0000);         /* フリーランニングカウンタをクリア */
    sfr_clr(ATU_TMDR3, ATU_TMDR_b);     /* PWM モードに設定 */
    sfr_outw(ATU_TIOR, 0xb000);         /* GRxD とコンペアマッチ、トグル出力 */
    sfr_set(ATU_TSTR1, ATU_TSTR_b);     /* TCNTフリーランカウンタ始動 */
#ifdef HOOK
    TICK_RATE = tc;
#endif
    return E_OK;
}

/*****************************************************************************
* インターバル割込み処理ルーチ（内蔵 ATU）
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clrw(ATU_TSR3, ATU_IMFD_b); /* IMID コンペアマッチフラグクリア */
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* システムクロック処理 */
}

/*****************************************************************************
* 周期タイマ割込みハンドラ (内蔵 CMT)
*
******************************************************************************/

#if (INTVSZ==0)
INTHDR inthdr(void)
{
    ent_int();                      /* 割込みハンドラの開始 */
    isritm(NULL);                   /* 割込サービスルーチンを呼ぶ */
    ret_int();                      /* 割込みハンドラから復帰する */
}
#endif

/*****************************************************************************
* チック入力積算値を算出（内蔵 ATU）
*
******************************************************************************/
#ifdef HOOK
#if HOOK==1
UW tickck_read(void)
#else
UW ctex_timer_read(void)
#endif
{
    UW clk;

    if (!(sfr_inw(ATU_TSR3) & ATU_IMFD_b))
    {   clk = sfr_inw(ATU_TCNT);
    }
    if (sfr_inw(ATU_TSR3) & ATU_IMFD_b)
    {   clk = sfr_inw(ATU_TCNT) + TICK_RATE;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#else

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_CMID, isritm, 0 };
#endif

UH calc_scale(UB *pscr, UH *tcr)
{
    static UB const prescale[] = { 1, 2, 4, 8, 16, 32 };
    UW clk,tc1, tc2;
    INT i, j;

    for (i = 0; i < sizeof(prescale); i++)
    {   tc1 = ((UW)(CLK) * MSEC)/((UW)prescale[i] * 1000 * 2);
        for(j = 0; j < 32; j++)
        {   tc2 = tc1 / (j+1);
            if (tc2 <= 0x10000L)
            {   *pscr = j;
                *tcr  = i << 12;
                return tc2;
            }
        }
    }
    return 0;
}

ER intsta(void)
{
    UH tc, tcr;
    UB pscr;

    /* 時定数を計算 */

    tc = calc_scale(&pscr, &tcr);
    if (tc == 0)
        return E_SYS;

    /* 割込み定義 */

  #if (INTVSZ!=0)
    acre_isr(&disr_isritm);
  #endif

    /* 割込み優先レベル設定 */

  #if (CH==6)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | (IP << 4));
  #elif (CH==7)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | IP);
  #endif

    /* タイマユニット初期化 */

    sfr_outw(ATU_CYLRD, tc);            /* 比較値 */
    sfr_out(ATU_PSCR, pscr);            /* 分周比選択 */
    sfr_outw(ATU_TCR, tcr);             /* クロック選択 */
    sfr_setw(ATU_TIER, 0x0008);         /* CMID 割込み許可 */
    sfr_outw(ATU_TCNT, 0x0001);         /* フリーランニングカウンタをクリア */
    sfr_set(ATU_TSTR2, ATU_TSTR_b);     /* TCNTフリーランカウンタ始動 */
#ifdef HOOK
    TICK_RATE = tc;
#endif
    return E_OK;
}

/*****************************************************************************
* インターバル割込み処理ルーチ（内蔵 ATU）
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clrw(ATU_TSR, 0x0008);      /* CMID コンペアマッチフラグクリア */
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* システムクロック処理 */
}

/*****************************************************************************
* 周期タイマ割込みハンドラ (内蔵 ATU)
*
******************************************************************************/

#if (INTVSZ==0)
INTHDR inthdr(void)
{
    ent_int();                      /* 割込みハンドラの開始 */
    isritm(NULL);                   /* 割込サービスルーチンを呼ぶ */
    ret_int();                      /* 割込みハンドラから復帰する */
}
#endif

/*****************************************************************************
* チック入力積算値を算出（内蔵 ATU）
*
******************************************************************************/

#ifdef HOOK
#if HOOK==1
UW tickck_read(void)
#else
UW ctex_timer_read(void)
#endif
{
    UW clk;

    if (!(sfr_inw(ATU_TSR) & 0x0008))
    {   clk = sfr_inw(ATU_TCNT) - 1;
    }
    if (sfr_inw(ATU_TSR) & 0x0008)
    {   clk = sfr_inw(ATU_TCNT) + TICK_RATE - 1;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#endif

#if defined(__GNUC__)   /* exeGCC */
/*****************************************************************************
* 割込み禁止／許可, ＶＢＲの設定／取得
*
******************************************************************************/

asm("
        .align  2
        .global _disable
_disable:
        mov     #0xf0,r0
        rts
        ldc     r0,sr

        .align  2
        .global _enable
_enable:
        mov     #0,r0
        rts
        ldc     r0,sr

        .align  2
        .global _set_cr
_set_cr:
        rts
        ldc     r4,sr

        .align  2
        .global _get_cr
_get_cr:
        rts
        stc     sr,r0

        .align  2
        .global _set_vbr
_set_vbr:
        rts
        ldc     r4,vbr

        .align  2
        .global _get_vbr
_get_vbr:
        rts
        stc     vbr,r0
");

#endif
#if defined(__ghs)      /* Green Hills */
/*****************************************************************************
* 割込み禁止／許可, ＶＢＲの設定／取得
*
******************************************************************************/

void disable(void)
{
    asm("   mov     #0xf0,r0");
    asm("   ldc     r0,sr   ");
}

void enable(void)
{
    asm("   mov     #0,r0   ");
    asm("   ldc     r0,sr   ");
}

void set_cr(int cr)
{
    asm("   ldc     r4,sr   ");
}

int get_cr(void)
{
    asm("   stc     sr,r0   ");
}

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

void **get_vbr(void)
{
    asm("   stc     vbr,r0  ");
}

#endif
#if defined(__TID__)    /* IAR EW/ICC */
/*****************************************************************************
* VBR handling (IAR EW/ICC)
*
******************************************************************************/

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

#endif

/* end */
