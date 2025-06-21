/*======================================================================
 * File name    : bkf100.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 * 	ＡＦＶ係数の変数定義
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/nz_wcu/wcu/rcs/bkf100.h 1.1 1998/03/09 15:43:25 ryoji Exp $
 *$Log: bkf100.h $
 * Revision 1.1  1998/03/09  15:43:25  ryoji
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/wcu/main/dz_wcu2/rcs/bkf100.h 1.1 1996/06/18 18:41:17 chimura Exp $
 *log: bkf100.h $
 * リビジョン 1.1  1996/06/18  18:41:17  chimura
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<drive\dadcdrv.h>		/* Show drive		*/
#include	"adcs.h"			/* Current		*/

#ifndef		BKF100_H
#define		BKF100_H

/*--------------------------------------------------------------*/
/*	AFV係数の構造体						*/
/*								*/
/*--------------------------------------------------------------*/

typedef struct afvt {
	long		org[DAD_WEI_MAX];	/* AFV係数の初期値	*/
	long		rev[DAD_WEI_MAX];	/* AFV係数の改訂値	*/
} AFVT;

/*--------------------------------------------------------------*/
/* 注)	既に納品されているユーザーに対して互換を維持する	*/
/*	ための対応策。						*/
/*	既知のAFV係数をafv_ini[]に記述し、ROM_BOOTを定義	*/
/*	することにより、EE-PROMにｺﾋﾟｰが生成される。		*/
/*								*/
#define	ROM_BOOT
/*--------------------------------------------------------------*/

#define	AFV_TYP_COEF	(60000L)		/* EX:AFV係数の代表値	*/

#if defined(STORAGE)
#ifdef	ROM_BOOT
const long	afv_ini[ADBRD_MAX][DAD_WEI_MAX] = {
/* AFV coef. typecal constant		*/
	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,
	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,
	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,
	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF,	AFV_TYP_COEF
};
#endif

#else
#ifdef	ROM_BOOT
extern const long	afv_ini[ADBRD_MAX][DAD_WEI_MAX];
#endif
extern struct afvt	*afvc[ADP_MAX];		/* AFV係数へのﾎﾟｲﾝﾀの[]	*/
#endif

#endif
