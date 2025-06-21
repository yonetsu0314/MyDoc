/*======================================================================
 * File name    : bkf004.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 * 	零点＆スパンのAD/DA変数定義
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/bkf004.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: bkf004.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/rz_wcu2/rcs/bkf004.h 1.1 1996/06/18 18:41:11
 *Log: bkf004.h
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

#ifndef		BKF004_H
#define		BKF004_H

#ifndef	ADCS_H
#include	"adcs.h"			/* Show Current		*/
#endif

/*--------------------------------------------------------------*/
/*	AD/DAデータの構造体					*/
/*								*/
/*--------------------------------------------------------------*/

typedef struct adda {
	short		ad[AD_CH_MAX];	/* ADカウントの配列		*/
	short		da[AD_CH_MAX];	/* DAデータの配列		*/
} ADDA;

#if defined(STORAGE)
struct	adda	adda_ini = { DEF_SPAN_AD, DEF_SPAN_DA };
#else
extern struct adda	adda_ini;
extern struct adda	*adzero[];	/* 零点AD/DAへのポインタ	*/
extern struct adda	*adspan[];	/* スパンAD/DAへのポインタ	*/
#endif

#endif
