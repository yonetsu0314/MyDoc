/*======================================================================
 * File name    : nzr055.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr055.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr055.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzr055.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr055.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR055_H)
#define	NZR055_H


/*------------------------------------------------------*/
/*	フィーダ共振周期設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzr055t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_natural_freq[32];	/* RF 共振周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_natural_freq[8];	/* DF 共振周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_natural_freq[8];	/* CF 共振周期		*/
} NZR055T;

#if	defined(STORAGE)
struct nzr055t	nzr055s;
#else
#if	!defined(NOEXTERN)
extern struct nzr055t	nzr055s;
#endif
#endif

#endif
