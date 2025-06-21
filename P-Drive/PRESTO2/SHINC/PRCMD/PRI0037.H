/*======================================================================
 * File name	: pri0037.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡモニター状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0037.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0037.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi037.h 1.2 1996/07/12 18:14:26 hiroki Exp
 *log: rzi037.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0037_H)
#define	PRI0037_H


/*------------------------------------------------------*/
/*	ＤＣＡモニター状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0037t {
	unsigned short	dca_sum_cnt;	/* DCAサンプルカウント値*/
	unsigned long	bf_cbwt;	/* 再チェック前重量　	*/
	unsigned long	af_cbwt;	/* 再チェック後重量　	*/
} PRI0037T;

#if	defined(STORAGE)
struct pri0037t	pri0037s;
#else
#if	!defined(NOEXTERN)
extern struct pri0037t	pri0037s;
#endif
#endif

#endif
