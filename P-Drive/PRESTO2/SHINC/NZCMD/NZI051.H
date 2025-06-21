/*======================================================================
 * File name    : nzi051.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi051.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi051.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi051.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzi051.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI051_H)
#define	NZI051_H


/*------------------------------------------------------*/
/*	フィーダ共振周期応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi051t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_res_cycle[32];	/* RF 共振周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_res_cycle[8];	/* DF 共振周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_res_cycle[8];	/* CF 共振周期		*/
} NZI051T;

#if	defined(STORAGE)
struct nzi051t	nzi051s;
#else
#if	!defined(NOEXTERN)
extern struct nzi051t	nzi051s;
#endif
#endif

#endif
