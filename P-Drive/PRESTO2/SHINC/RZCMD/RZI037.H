/*======================================================================
 * File name    : rzi037.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡモニター状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi037.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi037.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi037.h 1.2 1996/07/12 18:14:26 hiroki
 *Log: rzi037.h
 * リビジョン 1.2  1996/07/12  18:14:26  hiroki
 * ATA96仕様に変更。
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI037_H)
#define	RZI037_H


#include	<rzcmd/rzprm.h>

/*------------------------------------------------------*/
/*	ＤＣＡモニター状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi037t {
	unsigned int 		dca_sum_cnt;	/* DCAサンプルカウント値*/
	unsigned long int 	bf_cbwt;	/* 再チェック前重量　	*/
	unsigned long int 	af_cbwt;	/* 再チェック後重量　	*/
} RZI037T;

#if	defined(STORAGE)
struct rzi037t	rzi037s;
#else
#if	!defined(NOEXTERN)
extern struct rzi037t	rzi037s;
#endif
#endif

#endif
