/*======================================================================
 * File name	: pri0051.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0051.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0051.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi051.h 1.1 1970/01/01 00:00:00 kawamura Exp
 *log: nzi051.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0051_H)
#define	PRI0051_H


/*------------------------------------------------------*/
/*	フィーダ共振周期応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0051t {
	unsigned char	nof_rf;			/* number of RF		*/
	short		rf_res_cycle[32];	/* RF 共振周期		*/
	unsigned char	nof_df;			/* number of DF		*/
	short		df_res_cycle[8];	/* DF 共振周期		*/
	unsigned char	nof_af;			/* number of AF		*/
	short		af_res_cycle[32];	/* AF 共振周期		*/
	unsigned char	nof_cf;			/* number of CF		*/
	short		cf_res_cycle[8];	/* CF 共振周期		*/
} PRI0051T;

#if	defined(STORAGE)
struct pri0051t	pri0051s;
#else
#if	!defined(NOEXTERN)
extern struct pri0051t	pri0051s;
#endif
#endif

#endif
