/*======================================================================
 * File name    : intsta.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/dmu/RCS/intsta.c 1.5 1970/01/01 00:00:00 kagatume Exp $
 *$Log: intsta.c $
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2003/11/21 11:00
 * MSEC の定義を条件コンパイルで対応できるようにする
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2003/11/12 compiler ver7 vup
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ﾀｲﾏをcre_isr()関数を用いて生成
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * SH2用NORTiユーザ初期化関数
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
/* クロックの定義 */
#define CLK         29490000      /* Pφクロック[Hz] */

#include <kernel.h>
#include <nosys4.h>
#include <sh7615\sh7615.h>
#include <machine.h>

#ifdef MSEC
#undef	MSEC
#define MSEC        1             /* 1 msec */
#else
#define MSEC        1             /* 1 msec */
#endif

#define disable()   set_cr(15 << 4);
#define enable()    set_cr(0)

/* 割込み優先順位の定義 */
#define IP      14               /* IP */

/* チャネル番号の定義 */
#define CH      2               /* TPU2 */

#define TPU_TCR   TPU_TCR2
#define TPU_TMDR  TPU_TMDR2
#define TPU_TIOR  TPU_TIOR2
#define TPU_TIER  TPU_TIER2
#define TPU_TSR   TPU_TSR2
#define TPU_TCNT  TPU_TCNT2
#define TPU_TGRA  TPU_TGR2A
#define INT_TGIA  INT_TGI2A

void isritm(VP_INT exinf);
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_TGIA, isritm, 0 };
extern  void _kernel_interr(void);

ER intsta(void)
{
    static UH const prescale[] = { (UH)1, (UH)4, (UH)16, (UH)64 };
    UW tc;
    UB cks;

    /* 時定数を計算 */
    for (cks = (UB)0; cks <= (UB)3; cks++)
    {   tc = ((UW)(CLK) * MSEC)/((UW)prescale[cks] * 1000);
        if (tc <= 0xffffL)
            break;
    }
    if (cks > 3 || tc == 0L)
        return E_PAR; /* エラー */

	((UW *)0x6000000)[INT_TGIA] = (UW)_kernel_interr;

    /* 割込み定義 */
    acre_isr(&disr_isritm);

    /* 割込み優先レベル設定 */
    sfr_setw(INTC_IPRD, (IP) << (12 - CH * 4));

    /* 割込みベクタ番号設定 */
    sfr_outw((unsigned)INTC_VCRJ, (UH)(sfr_inw(INTC_VCRJ) | (INT_TGIA << 8)));

    /* タイマ初期化 */
    sfr_out((unsigned)TPU_TCR, (UB)(0x20+cks));     /* クロック選択＆コンペアマッチＡでクリア */
    sfr_outw(TPU_TGRA, (UH)tc);     /* アウトプットコンペア値 */
    sfr_clr(TPU_TSR, 0x01);         /* アウトプットコンペアフラグＡクリア*/
    sfr_outw((unsigned)TPU_TCNT, (UH)0x00);       /* カウンタクリア */
    sfr_set(TPU_TIER, 0x01);        /* TGIEA 割り込み許可 */
    sfr_clr(TPU_TSYR, 0x01 << CH);  /* カウント独立 */
    sfr_set(TPU_TSTR, 0x01 << CH);  /* カウント動作スタート */

    return E_OK;
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
//    return E_NOSPT;
    return E_OK;
}
