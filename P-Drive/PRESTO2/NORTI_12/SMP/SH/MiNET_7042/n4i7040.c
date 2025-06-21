/******************************************************************************
* インターバル割込みハンドラ (SH7040シリーズ)                                 *
*                                                                             *
*  File name : n4i7040.c                                                      *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                   *
*                                                                             *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=?,CH=? n4i7040.c          (Renesas C) *
*            gcc -c -m2 -O2 -DCLK=? -DCH=? n4i7040.c              (exeGCC)    *
*            ccsh -c -cpu=sh_ii -ansi -noasmwarn -O -DCLK=?                   *
*                                               -DCH=? n4i7040.c  (GHS)       *
*                                                                             *
* 24/Jul/1998                                                                 *
* 30/Nov/1998 MiNET-7042 用に修正                                             *
* 12/Sep/2000 フックルーチン用の積算Tickクロック読み出しを追加             TA *
* 30/Oct/2000 IAR EW 対応                                                     *
* 19/Sep/2001 処理系の判断を見直し, フックルーチン用のタイムスタンプを追加 TA *
* 23/Dec/2002 INTVSZ=0対応漏れ修正, inthdrを割込みベクタ(ROM)に記述可      MI *
* 23/Dec/2002 SH7042/44/45で共通のためn4i7040.cとする, コメント修正        MI *
******************************************************************************/

/*
割込みベクタテーブル

          def_inhシステムコールによる割込みベクタの設定を可能にするため、
          RAM先頭(400000H番地から1024バイト)に、割込みベクタテーブルがあると
          想定しています。
          割込みベクタテーブルの領域が、他の領域と重ならない様にリンクしてく
          ださい。(重なってもリンカで警告は出ません!)
          割込みベクタテーブルの先頭アドレスはINTVECマクロで変更できます。
          割込みベクタテーブルのサイズ(バイト数)は、INTVSZクロで変更できます。

          (例) shc <op> -def=INTVEC=0,INTVSZ=336 n4i7040.c … 0番地,336バイト
                             ~~~~~~~~ ~~~~~~~~~~
          (例) gcc <op> -DINTVEC=0 -DINTVSZ=336 n4i7040.c
                          ~~~~~~~~   ~~~~~~~~~~
          割込みベクタテーブルがROM上にあり、def_inhに依らず直接ベクタを定義
          する場合、INTVEC=0, INTVSZ=0としてください。この場合、def_inhシステ
          ムコールは機能しません。

dis_int, ena_int

          SHシリーズの割込みコントローラでは、汎用的なdis_int, ena_intシステ
          ムコールの実装が複雑となってしまいます。
          この２つのシステムコールは用意していませんので、個別の割込み禁止許
          可を行いたい場合は、各制御レジスタを直接アクセスしてください。

_kernel_inROM

          与えられたアドレスがROM領域か否かを返す関数です。ROM領域であれば
          TRUE(1), ROM領域でなければ FALSE(0) を返します。
*/

#include "kernel.h"
#include "nosys4.h"
#include "sh7040.h"

/* 割込みベクタテーブル */

#ifndef INTVEC
#define INTVEC      0x400000L   /* 先頭アドレス未定義の場合 */
#endif
#ifndef INTVSZ
#define INTVSZ      1024        /* サイズ未定義の場合 */
#endif

/* 割込み優先順位の定義 */

#ifndef IP
#define IP      7               /* 割込み優先順位未定義の場合 */
#endif

/* クロックの定義 */

#define MHz         1000000L
#ifndef CLK
#define CLK         24*MHz      /* クロック未定義の場合 */
#endif

/* チャネル番号の定義 */

#ifndef CH
#define CH      0               /* チャネル番号未定義の場合 */
#endif

/* CHに合わせたCMTレジスタの再定義 */

#if (CH==0)
#define CMT_CMCSR   CMT_CMCSR0
#define CMT_CMCNT   CMT_CMCNT0
#define CMT_CMCOR   CMT_CMCOR0
#define INT_CMT     INT_CMI0
#elif (CH==1)
#define CMT_CMCSR   CMT_CMCSR1
#define CMT_CMCNT   CMT_CMCNT1
#define CMT_CMCOR   CMT_CMCOR1
#define INT_CMT     INT_CMI1
#else
#error illegal CH number! (CH = 0 to 1)
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

#elif defined(GAIO)             /* XASS-V */
#include <ht1_inline.h>
extern UW CONST_TOP[], CONST_SIZE[];

#else                           /* Renesas C */
#ifndef __HITACHI__
#define __HITACHI__
#endif
#include <machine.h>
#define disable()   set_cr(15 << 4);
#define enable()    set_cr(0)
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;
#if (INTVSZ==0)
#pragma interrupt(inthdr)
#endif
#endif

/* Tick入力積算カウンタ */

#ifdef HOOK
UW TOTAL_TICKCK;
UW TICK_RATE;
#endif

/* 外部参照 */

extern int RST_VECT;
extern UB _KERNEL_pisrposi;
extern UB _KERNEL_hdrlen;
extern void _kernel_interr(void);

/* 割込みサービスルーチン生成情報 */

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_CMT, isritm, 0 };
#endif

/*****************************************************************************
* 割込み初期化
*
* 形式  ER intini(void);
*
* 戻値  常に E_OK
*
* 解説  カーネルの初期化ルーチン sysini から割込み禁止状態で呼び出されます。
******************************************************************************/


ER intini(void)
{
  #if (INTVSZ!=0) /* 割込みテーブルをROM上に定義した場合は INTVSZ == 0 */

    INT i;
    int *p;

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
* 形式  BOOL _kernel_inROM(const void *p);
*
*       p     チェックする領域へのポインタ
*
* 戻値  TRUE  コード領域または定数領域
*       FALSE 上記以外
*
* 解説  オブジェクト生成情報等が定数か変数かを判定するために、カーネルの各種
*       システムコールから呼び出されます。
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
* 割込みサービスルーチン生成情報のチェック
*
* 形式  ER _kernel_check_cisr(const T_CISR *pk_cisr);
*
*       pk_cisr  チェックする割込みサービスルーチン生成情報へのポインタ
*
* 戻値  E_PAR 割込み番号とマスクレベルのいずれかが範囲外
*       E_OK  割込み番号とマスクレベルは共に正常
*
* 解説  割込みサービスルーチン生成情報の割込み番号とマスクレベルの範囲を
*       チェックします。
*       cre_isr, acre_isr システムコールから呼び出されます。
******************************************************************************/

ER _kernel_check_cisr(const T_CISR *pk_cisr)
{
    if (pk_cisr->intno >= (INTVSZ) / 4)
        return E_PAR;
    return E_OK;
}

/*****************************************************************************
* 割込みハンドラのアドレス設定
*
* 形式  void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr);
*
*       hdr       割込みサービスルーチンを呼び出す割込みハンドラのアドレス
*       pk_cisr   チェックする割込みサービスルーチン生成情報へのポインタ
*
* 解説  割込みサービスルーチンを呼び出すために、内部で生成される割込みハンド
*       ラのアドレスを、割込みサービスルーチン生成情報の割込み番号より算出し
*       た割込みベクタへ格納します。
*       cre_isr, acre_isr システムコールから呼び出されます。
******************************************************************************/

void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    *p = (UW)hdr;
}

/*****************************************************************************
* 割込みハンドラのアドレス取得
*
* 形式  FP _kernel_get_inthdr(const T_CISR *pk_cisr);
*
*       pk_cisr  チェックする割込みサービスルーチン生成情報へのポインタ
*
* 戻値  当該割込み番号の割込みハンドラアドレス
*
* 解説  割込みサービスルーチン生成情報の割込み番号より算出した割込みベクタか
*       ら、割込みハンドラアドレスを読み出します。
*       cre_isr, acre_isr システムコールから呼び出されます。
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
* 形式  ER def_inh(UINT inhno, const T_DINH *pk_dinh);
*
*       inhno    割込みベクタ番号
*       pk_dinh  割込みハンドラ定義情報
*
* 戻値  エラーコード
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

    /* 定義解除の場合、未定義割込みハンドラを設定 */

    if (pk_dinh == (T_DINH *)NADR)
        fp = (FP)_kernel_interr;
    else
        fp = pk_dinh->inthdr;

    /* CPU割込み禁止 */

    psw = vdis_psw();

    /* 新しい割込みベクタ設定 */

    *p = (UW)fp;

    /* CPU割込み禁止戻す */

    vset_psw(psw);
    return E_OK;

  #endif
}

/*****************************************************************************
* 周期タイマ割込み起動 (内蔵CMT)
*
* 形式  ER intsta(void);
*
* 戻値  エラーコード
*
* 補足  割込み禁止状態で実行してください。
******************************************************************************/

ER intsta(void)
{
    static UH const prescale[] = { 8, 32, 128, 512 };
    UW tc;
    UH cks;

    /* 時定数を計算 */

    for (cks = 0; cks <= 3; cks++) {
        tc = ((UW)(CLK) * MSEC)/((UW)prescale[cks] * 1000);
        if (tc <= 0xffffL)
            break;
    }
    if (cks > 3 || tc == 0L)
        return E_PAR; /* エラー */

    /* 割込み定義 */

  #if (INTVSZ!=0) /* ROMに定義する場合は不要 */
    acre_isr(&disr_isritm);
  #endif

    /* 割込み優先レベル設定 */

    sfr_setw(INTC_IPRG, (IP) << (4 - CH * 4));

    /* タイマ初期化 */

    sfr_outw(CMT_CMCOR, (H)(tc-1)); /* 比較値 */
    sfr_outw(CMT_CMCSR, 0x40        /* コンペアマッチ割り込みを許可 */
                       |cks);       /* クロック選択 */
    sfr_setw(CMT_CMSTR, 0x01 << CH);/* カウント動作スタート */
  #ifdef HOOK
    TICK_RATE = (UW)tc;
  #endif

    return E_OK;
}

/*****************************************************************************
* 周期タイマ割込サービスルーチン (内蔵 CMT)
*
******************************************************************************/

void isritm(VP_INT exinf)
{
  #ifdef HOOK
    UINT psw;
    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
  #endif
    sfr_clrw(CMT_CMCSR, 0x80);      /* CMF コンペアマッチフラグクリア */
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
* Tick入力積算値を算出 (内蔵CMT)
*
******************************************************************************/

#ifdef HOOK
#if (HOOK==1)
UW tickck_read(void)
#else
UW ctex_timer_read(void)
#endif
{
    UW clk;
    float wclk;

    if (!(sfr_inw(CMT_CMCSR) & 0x80))
        clk = sfr_inw(CMT_CMCNT) + TOTAL_TICKCK;
    if (sfr_inw(CMT_CMCSR) & 0x80)
        clk = sfr_inw(CMT_CMCNT) + TICK_RATE + TOTAL_TICKCK;

    /* ここを有効にすれば単位通りのタイムスタンプになるが、浮動小数点を */
    /* 使用するためオーバーヘッドが大きくなる。 */
  #if 0
    wclk = clk * MSEC * 1000.0;
    clk = (UW)(wclk / TICK_RATE);
  #endif
    return clk;
}

#else

UW tickck_read(void)
{
    return 0L;
}
#endif

#if defined(__GNUC__)   /* exeGCC */
/*****************************************************************************
* 割込み禁止／許可, VBRの設定／取得
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
#if defined(__ghs)   /* Green Hills */
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
