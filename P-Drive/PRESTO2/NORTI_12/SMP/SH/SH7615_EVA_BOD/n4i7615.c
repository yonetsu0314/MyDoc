/****************************************************************************
* インターバル割込みハンドラ（SH7615）                                      *
*                                                                           *
*  File name : n4i7615.c                                                    *
*  Copyright (c) 2000-2002 MiSPO Co., Ltd.  All rights reserved.            *
*                                                                           *
* Compile : shc -cp=sh2 -op=1 -def=CLK=?,CH=? n4i7615.c         (Renesas C) *
*           gcc -c -m2 -O2 -DCLK=? -DCH=? n4i7615.c             (exeGCC)    *
*           ccsh -c -cpu=sh_ii -ansi -noasmwarn -O -DCLK=? -DCH=? n4i7615.c *
*                                                               (GHS)       *
*                                                                           *
* 11/Apr/2000 作成                                                    MiSPO *
* 24/Sep/2000 フックルーチン用のダミー積算チッククロックを修正              *
* 30/Oct/2000 IAR EW 対応                                                   *
* 19/Sep/2001 制御系の判断を見直し、フックルーチンのタイムスタンプを追加 TA *
* 27/Dec/2002 INTVSZ=0対応漏れ修正, inthdrを割込みベクタ(ROM)に記述可    KA *
* 29/Jul/2003 ベクタROM定義時の見直し                                    KA *
****************************************************************************/

/*

使用するハードウェア

          SH7615 内蔵 FRT または TPU をインターバル･タイマとして使用して
          います。
          それ以外のタイマを用いる場合は、ユーザーで改造してください。

割込み周期の指定

          割込み周期は、MSEC マクロで決まります。デフォルトは 10 msec です。
          例えば、5 msec に変更する場合は、次の様にコンパイルしてください。

          (例) shc -cp=sh2 -op=1 -def=MSEC=5 n4i7615.c
                                      ~~~~~~
          (例) gcc -c -m2 -O2 -DMSEC=5 n4i7615.c
                                ~~~~~~
クロックの指定

          ソースクロック(Pφ)のデフォルトは 15 MHz です。変更する場合 は
          CLK マクロを定義してコンパイルしてください。値は [Hz] 単位の
          周波数です。

          (例) shc <options> -def=CLK=28700000 n4i7615.c ･････ 28.7 MHz
                                  ~~~~~~~~~~~~
          (例) gcc <options> -DCLK=28700000 n4i7615.c
                               ~~~~~~~~~~~~
チャネル番号の指定

          CH マクロで、使用する TPU チャネル（CH=0～2）
          または、              FRT         （CH=3）
          を指定できます。
          デフォルトは CH=3 です。

          (例) shc <options> -def=CH=1 n4i7615.c  ･･････････ TPU ﾁｬﾈﾙ1
                                  ~~~~
          (例) gcc <options> -DCH=1 n4i7615.c
                               ~~~~
割込みベクタテーブル

          def_inh システムコールによる割込みベクタの設定を可能にするため、
          RAM 先頭（6000000H 番地から 1024 バイト）に、割込みベクタテーブルが
          あると想定しています。
          割込みベクタテーブルの領域が、他の領域と重ならない様にリンクしてく
          ださい。（重なってもリンカで警告は出ません!）
          割込みベクタテーブル先頭アドレスは INTVEC マクロで、サイズ（バイト
          数）は、INTVSZ マクロで変更できます。

          (例) shc <op> -def=INTVEC=0,INTVSZ=336 n4i7615.c ･･･ 0番地,336ﾊﾞｲﾄ
                             ~~~~~~~~ ~~~~~~~~~~
          (例) gcc <op> -DINTVEC=0 -DINTVSZ=336 n4i7615.c
                          ~~~~~~~~   ~~~~~~~~~~
          割込みベクタテーブルが ROM 上にあり、def_inh に依らず直接ベクタを定
          義する場合、INTVSZ=0 としてください。
          この場合、def_inh システムコールは機能しません。

割込み優先順位

          本ハンドラの、デフォルトの割込み優先順位は、7 です。
          これを変更する場合、IP マクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。（IP ≦ KNL_LEVEL）
          本ハンドラより高優先度の割込みハンドラが有る場合、多重割込みが起こ
          ります。

          (例) shc <options> -def=IP=2 n4i7615.c ･･･ 割込み優先順位 2
                                  ~~~~
          (例) gcc <options> -DIP=2 n4i7615.c
                               ~~~~
dis_int, ena_int

          割り込みレベル設定のビット配置が不規則な SH7615 の割込みコントロー
          ラでは、汎用的な dis_int, ena_int システムコールの実装が複雑となっ
          てしまいます。
          この２つのシステムコールは用意していませんので、個別の割込み禁止許
          可を行いたい場合は、各制御レジスタを直接アクセスしてください。

_kernel_inROM(const void *)

          与えられたアドレスがROM領域か否かを返す関数です。ROM領域であれば
          TRUE(1), ROM領域でなければ FALSE(0) を返してください。

*/

#include "kernel.h"
#include "nosys4.h"
#include "sh7615.h"

/* 割込みベクタテーブル */

#ifndef INTVEC
#define INTVEC      0x6000000L
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
#define CLK         15*MHz      /* Pφクロック[Hz] */
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

#elif defined(__TID__)          /* IAR EW/ICC */
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
UH CM_FLAG;
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
    int *p;

    /* 割込みベクタテーブルの初期化 */

    for (i = INT_IRL1; i < (INTVSZ) / 4; i++)
        ((UW *)INTVEC)[i] = (UW)_kernel_interr;

    /* ベクタベースレジスタを設定 */
    /* リモートデバッガ(リセットベクタが0番地でない場合)は除く */

    p = &RST_VECT;
    if (p == (int *)0L)
        set_vbr((void **)INTVEC);
    else{
      #ifdef REBOOT
        set_vbr((void **)INTVEC);
      #endif
    }

  #endif

  #ifdef HOOK
    TOTAL_TICKCK = 0;
    CM_FLAG = 0;
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

#elif defined(__TID__)          /* IAR EW/ICC */
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

#if CH < 3
/*****************************************************************************
* インターバルタイマ起動（SH7615 内蔵 TPU）
*
* 割込み禁止状態で実行してください。
******************************************************************************/

#if (CH==0)
#define TPU_TCR   TPU_TCR0
#define TPU_TMDR  TPU_TMDR0
#define TPU_TIOR  TPU_TIOR0H
#define TPU_TIER  TPU_TIER0
#define TPU_TSR   TPU_TSR0
#define TPU_TCNT  TPU_TCNT0
#define TPU_TGRA  TPU_TGR0A
#define INT_TGIA  INT_TGI0A
#elif (CH==1)
#define TPU_TCR   TPU_TCR1
#define TPU_TMDR  TPU_TMDR1
#define TPU_TIOR  TPU_TIOR1
#define TPU_TIER  TPU_TIER1
#define TPU_TSR   TPU_TSR1
#define TPU_TCNT  TPU_TCNT1
#define TPU_TGRA  TPU_TGR1A
#define INT_TGIA  INT_TGI1A
#else
#define TPU_TCR   TPU_TCR2
#define TPU_TMDR  TPU_TMDR2
#define TPU_TIOR  TPU_TIOR2
#define TPU_TIER  TPU_TIER2
#define TPU_TSR   TPU_TSR2
#define TPU_TCNT  TPU_TCNT2
#define TPU_TGRA  TPU_TGR2A
#define INT_TGIA  INT_TGI2A
#endif

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_TGIA, isritm, 0 };
#endif

ER intsta(void)
{
    static UH const prescale[] = { 1, 4, 16, 64 };
    UW tc;
    UB cks;

    /* 時定数を計算 */

    for (cks = 0; cks <= 3; cks++)
    {   tc = ((UW)(CLK) * MSEC)/((UW)prescale[cks] * 1000);
        if (tc <= 0xffffL)
            break;
    }
    if (cks > 3 || tc == 0L)
        return E_PAR; /* エラー */

    /* 割込み定義 */

  #if (INTVSZ!=0)
    /* 割込み定義: ROMに定義する場合は不要 */
    acre_isr(&disr_isritm);
  #endif

    /* 割込み優先レベル設定 */

    sfr_setw(INTC_IPRD, (IP) << (12 - CH * 4));

    /* 割込みベクタ番号設定 */

  #if INT_TGIA == INT_TGI0A
    sfr_outw(INTC_VCRE, sfr_inw(INTC_VCRE) | (INT_TGIA << 8));
  #elif INT_TGIA == INT_TGI1A
    sfr_outw(INTC_VCRH, sfr_inw(INTC_VCRH) | (INT_TGIA << 8));
  #else
    sfr_outw(INTC_VCRJ, sfr_inw(INTC_VCRJ) | (INT_TGIA << 8));
  #endif

    /* タイマ初期化 */

    sfr_out(TPU_TCR, 0x20+cks);     /* クロック選択＆コンペアマッチＡでクリア */
    sfr_outw(TPU_TGRA, (UH)tc);     /* アウトプットコンペア値 */
    sfr_clr(TPU_TSR, 0x01);         /* アウトプットコンペアフラグＡクリア*/
    sfr_outw(TPU_TCNT, 0x00);       /* カウンタクリア */
    sfr_set(TPU_TIER, 0x01);        /* TGIEA 割り込み許可 */
    sfr_clr(TPU_TSYR, 0x01 << CH);  /* カウント独立 */
    sfr_set(TPU_TSTR, 0x01 << CH);  /* カウント動作スタート */
#ifdef HOOK
    TICK_RATE = (UW)tc;
#endif

    return E_OK;
}

/*****************************************************************************
* インターバル割込処理ルーチン（内蔵 TPU）
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clr(TPU_TSR, 0x01);         /* アウトプットコンペアフラグＡクリア*/
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* システムクロック処理 */
}

/*****************************************************************************
* 周期タイマ割込みハンドラ (内蔵 TPU)
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
* チック入力積算値を算出（ダミー）
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

    if (!(sfr_in(TPU_TSR) & 0x01))
    {   clk = sfr_inw(TPU_TCNT);
    }
    if (sfr_in(TPU_TSR) & 0x01)
    {   clk = sfr_inw(TPU_TCNT) + TICK_RATE;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#elif CH == 3
/*****************************************************************************
* インターバルタイマ起動（SH7615 内蔵 FRT）
*
* 割込み禁止状態で実行してください。
******************************************************************************/

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_OCI, isritm, 0 };
#endif

ER intsta(void)
{
    UH tc;
    UB cks;

    /* 時定数を計算 */

  #if (((CLK) * MSEC)/(1000L * 8) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 8));
    cks = 0x00;
  #elif (((CLK) * MSEC)/(1000L * 32) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 32));
    cks = 0x01;
  #elif (((CLK) * MSEC)/(1000L * 128) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 128));
    cks = 0x02;
  #else
    #error time constant overflow!
  #endif

    /* 割込み定義 */

  #if (INTVSZ!=0)
    /* 割込み定義: ROMに定義する場合は不要 */
    acre_isr(&disr_isritm);
  #endif

    /* 割込み優先レベル設定 */

    sfr_outw(INTC_IPRB, sfr_inw(INTC_IPRB) | (IP << 8));

    /* 割込みベクタ番号設定 */

    sfr_outw(INTC_VCRC, sfr_inw(INTC_VCRC) | INT_OCI);

    /* フリー･ランニング･タイマ初期設定 */

    sfr_clr(FRT_TOCR, 0x10);            /* OCRA 選択 */
    sfr_out(FRT_OCRABH, tc>>8);         /* アウトプットコンペア値 */
    sfr_out(FRT_OCRABL, tc&0xff);
    sfr_out(FRT_TCR, cks);              /* クロック選択 */
    sfr_out(FRT_FTCSR, 0x01);           /* コンペアマッチＡでクリア */
    sfr_clr(FRT_FTCSR, 0x08);           /* アウトプットコンペアフラグＡクリア*/
    sfr_out(FRT_FRCH, 0x00);            /* カウンタクリア */
    sfr_out(FRT_FRCL, 0x00);
    sfr_out(FRT_TIER, 0x08);            /* OCIA 割込み許可 */
#ifdef HOOK
    TICK_RATE = (UW)tc+1;
#endif
    return E_OK;
}

/*****************************************************************************
* インターバル割込処理ルーチン（内蔵 FRT）
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
    CM_FLAG = 0;
#endif
    sfr_clr(FRT_FTCSR, 0x08);       /* アウトプットコンペアフラグＡクリア*/
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* システムクロック処理 */
}

/*****************************************************************************
* 周期タイマ割込みハンドラ (内蔵 FRT)
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
* チック入力積算値を算出（内蔵 FRT）
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

    if (sfr_in(FRT_TIER) & 0x08)
    {
        if (!(sfr_in(FRT_FTCSR) & 0x08))
        {   clk  = sfr_in(FRT_FRCH) << 8;
            clk += sfr_in(FRT_FRCL);
        }
        if ((sfr_in(FRT_FTCSR) & 0x08) || (CM_FLAG))
        {   CM_FLAG = 1;
            clk  = sfr_in(FRT_FRCH) << 8;
            clk += sfr_in(FRT_FRCL);
            clk += TICK_RATE;
        }
        return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
    }
    else
    {
        return 0L;
    }
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#else
#error illegal CH number! (CH = 0 to 3)
#endif  /* #if CH == ?? */

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
* VBR and GBR handling (IAR EW/ICC)
*
******************************************************************************/

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

#endif

/* end */
