/*======================================================================
 * File name    : nzi011.h
 * Original	: rzi011.h
 *----------------------------------------------------------------------
 * Function	:
 *	秤データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi011.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi011.h 1.1 1999/05/27 16:35:08 kawamura
 *Log: nzi011.h
 * リビジョン 1.1  1999/05/27  16:35:08  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI011_H)
#define	NZI011_H


typedef struct nzi0110t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} NZI0110T;

/*------------------------------------------------------*/
/*	秤データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi011t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct nzi0110t	wh[32];		/* WH重量&ステータス[nof_wh]	*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	struct nzi0110t	bh[32];		/* WH重量&ステータス[nof_bh]	*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct nzi0110t	df[8];		/* WH重量&ステータス[nof_df]	*/
} NZI011T;

#if	defined(STORAGE)
struct nzi011t	nzi011s;
#else
#if	!defined(NOEXTERN)
extern struct nzi011t	nzi011s;
#endif
#endif

#endif
