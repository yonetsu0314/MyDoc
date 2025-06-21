/*======================================================================
 * File name	: pri0011.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0011.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi011.h 1.1 1999/05/27 16:35:08 kawamura Exp
 *log: nzi011.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0011_H)
#define	PRI0011_H


typedef struct pri00110t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} PRI00110T;

enum pri00110e {			/* データの識別			*/
	REAL_DATA,			/* 要求に応じてサンプリングしたデータ	*/
	CAL_DATA,			/* 組み合わせ計算で使用したデータ	*/
};
/*------------------------------------------------------*/
/*	秤データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0011t {
	unsigned char	ch_id;		/* channel number		*/
	enum pri00110e	flag;		/* real data or cal. data	*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct pri00110t	wh[32];	/* WH重量&ステータス[nof_wh]	*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	struct pri00110t	bh[32];	/* BH重量&ステータス[nof_bh]	*/
	unsigned char	nof_ph;		/* 該当するchのBH数		*/
	struct pri00110t	ph[32];	/* PH重量&ステータス[nof_bh]	*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct pri00110t	df[8];	/* WH重量&ステータス[nof_df]	*/
} PRI0011T;

#if	defined(STORAGE)
struct pri0011t	pri0011s;
#else
#if	!defined(NOEXTERN)
extern struct pri0011t	pri0011s;
#endif
#endif

#endif
