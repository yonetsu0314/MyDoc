/*======================================================================
 * File name    : rzi011.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi011.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi011.hv  2.1  93/11/02 15:40:14  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi011.hv
 * Revision 2.1  93/11/02 15:40:14  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI011_H)
#define	RZI011_H


typedef struct rzi0110t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} RZI0110T;

/*------------------------------------------------------*/
/*	秤データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi011t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct rzi0110t	wh[32];		/* WH重量&ステータス[nof_wh]	*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	struct rzi0110t	bh[32];		/* WH重量&ステータス[nof_bh]	*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct rzi0110t	df[8];		/* WH重量&ステータス[nof_df]	*/
} RZI011T;

#if	defined(STORAGE)
struct rzi011t	rzi011s;
#else
#if	!defined(NOEXTERN)
extern struct rzi011t	rzi011s;
#endif
#endif

#endif
