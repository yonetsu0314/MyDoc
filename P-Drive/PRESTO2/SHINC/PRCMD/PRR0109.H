/*======================================================================
 * File name	: prr0109.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0109.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0109.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr053.h 1.1 1970/01/01 00:00:00 kawamura Exp
 *log: nzr053.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0109_H)
#define	PRR0109_H


/*------------------------------------------------------*/
/*	フィーダ駆動周期設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0109t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_drv_cycle[32];	/* RF 駆動周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_drv_cycle[8];	/* DF 駆動周期		*/
	unsigned char	nof_af;			/* number of AF		*/
	short		af_drv_cycle[32];	/* AF 駆動周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_drv_cycle[8];	/* CF 駆動周期		*/
} PRR0109T;

#if	defined(STORAGE)
struct prr0109t	prr0109s;
#else
#if	!defined(NOEXTERN)
extern struct prr0109t	prr0109s;
#endif
#endif

#endif
