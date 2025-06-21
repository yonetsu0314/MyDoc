/*======================================================================
 * File name    : rzi111.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張秤データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi111.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi111.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi111.h 1.1 1970/01/01 00:00:00 itami
 *Log: rzi111.h
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI111_H)
#define	RZI111_H


typedef struct rzi1110t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} RZI1110T;

/*------------------------------------------------------*/
/*	秤データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi111t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_ph_no;	/* 最小WH番号			*/
	unsigned char	nof_ph;		/* 該当するchのWH数		*/
	struct rzi1110t	ph[32];		/* WH重量&ステータス[nof_ph]	*/
} RZI111T;

#if	defined(STORAGE)
struct rzi111t	rzi111s;
#else
#if	!defined(NOEXTERN)
extern struct rzi111t	rzi111s;
#endif
#endif

#endif
