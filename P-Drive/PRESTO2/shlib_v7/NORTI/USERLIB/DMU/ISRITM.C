/*======================================================================
 * File name    : isritm.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/dmu/RCS/isritm.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: isritm.c $
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
#include <kernel.h>
#include <sh7615\sh7615.h>

#define TPU_TCR   TPU_TCR2
#define TPU_TMDR  TPU_TMDR2
#define TPU_TIOR  TPU_TIOR2
#define TPU_TIER  TPU_TIER2
#define TPU_TSR   TPU_TSR2
#define TPU_TCNT  TPU_TCNT2
#define TPU_TGRA  TPU_TGR2A
#define INT_TGIA  INT_TGI2A

/*****************************************************************************
* インターバル割込処理ルーチン（内蔵 TPU）
*
******************************************************************************/
void isritm(VP_INT exinf)
{
    sfr_clr(TPU_TSR, 0x01);         /* アウトプットコンペアフラグＡクリア*/
    sig_tim();                      /* システムクロック処理 */
}


