/*======================================================================
 * File name    : nzr053.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr053.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr053.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr053.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr053.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR053_H)
#define	NZR053_H


/*------------------------------------------------------*/
/*	フィーダ駆動周期設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzr053t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_drv_cycle[32];	/* RF 駆動周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_drv_cycle[8];	/* DF 駆動周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_drv_cycle[8];	/* CF 駆動周期		*/
} NZR053T;

#if	defined(STORAGE)
struct nzr053t	nzr053s;
#else
#if	!defined(NOEXTERN)
extern struct nzr053t	nzr053s;
#endif
#endif

#endif
