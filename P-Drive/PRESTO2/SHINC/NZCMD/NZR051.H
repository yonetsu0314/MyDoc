/*======================================================================
 * File name    : nzr051.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr051.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr051.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzr051.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr051.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR051_H)
#define	NZR051_H


/*------------------------------------------------------*/
/*	フィーダ共振周期検出開始パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/


typedef struct nzr051t {
	unsigned long	rf_detect_ptn;	/* RF detection pattern	*/
	unsigned long	df_detect_ptn;	/* DF detection pattern	*/
	unsigned long	cf_detect_ptn;	/* CF detection pattern	*/
} NZR051T;

#if	defined(STORAGE)
struct nzr051t	nzr051s;
#else
#if	!defined(NOEXTERN)
extern struct nzr051t	nzr051s;
#endif
#endif

#endif
