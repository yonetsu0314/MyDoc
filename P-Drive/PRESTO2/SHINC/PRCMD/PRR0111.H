/*======================================================================
 * File name	: prr0111.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期設定データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0111.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0111.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr055.h 1.1 1970/01/01 00:00:00 kawamura Exp
 *log: nzr055.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0111_H)
#define	PRR0111_H


/*------------------------------------------------------*/
/*	フィーダ共振周期設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0111t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_natural_freq[32];	/* RF 共振周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_natural_freq[8];	/* DF 共振周期		*/
	unsigned char	nof_af;			/* number of AF		*/
	short		af_natural_freq[32];	/* AF 共振周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_natural_freq[8];	/* CF 共振周期		*/
} PRR0111T;

#if	defined(STORAGE)
struct prr0111t	prr0111s;
#else
#if	!defined(NOEXTERN)
extern struct prr0111t	prr0111s;
#endif
#endif

#endif
