/*======================================================================
 * File name    : rzi113.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	状態データ拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi113.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi113.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzi113.h 1.1 1994/07/15 14:50:02 fukutome
 *Log: rzi113.h
 * リビジョン 1.1  1994/07/15  14:50:02  fukutome
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI113_H)
#define	RZI113_H

#include	<rzcmd/rzi013.h>

/*------------------------------------------------------*/
/*	分散フィーダ供給状態のパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi113t {
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* DFの数			*/
	enum rzi0132e	infeed_stat[8];	/* 供給状態			*/
} RZI113T;

#if	defined(STORAGE)
struct rzi113t	rzi113s;
#else
#if	!defined(NOEXTERN)
extern struct rzi113t	rzi113s;
#endif
#endif

#endif	/* RZI113_H */
