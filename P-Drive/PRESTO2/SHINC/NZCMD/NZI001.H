/*======================================================================
 * File name    : nzi001.h
 * Original	: dzi205.h
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi001.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi001.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi001.h 1.1 1999/05/27 16:35:04 kawamura
 *Log: nzi001.h
 * リビジョン 1.1  1999/05/27  16:35:04  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI001_H)
#define	NZI001_H

typedef struct nzi0010t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} NZI0010T;

/*------------------------------------------------------*/
/*	秤診断データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi001t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct nzi0010t	wh_3rd[32];	/* 3段目重量&ステータス[nof_wh]	*/
	struct nzi0010t	wh_4th[32];	/* 4段目重量&ステータス[nof_bh]	*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct nzi0010t	df[8];		/* WH重量&ステータス[nof_df]	*/
	unsigned char	nof_afv;	/* ＡＦＶセル数			*/
	unsigned short	afv_out[8];	/* ＡＦＶセル出力		*/
} NZI001T;

#if	defined(STORAGE)
struct nzi001t	nzi001s;
#else
#if	!defined(NOEXTERN)
extern struct nzi001t	nzi001s;
#endif
#endif

#endif
