/*======================================================================
 * File name    : dzi205.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzi205.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzi205.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzi205.hv  1.4  94/03/23 18:05:12  kabumoto
 *Log: P:/INCLUDE/DZCMD/RCS/dzi205.hv
 * Revision 1.4  94/03/23 18:05:12  kabumoto
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZI205_H)
#define	DZI205_H

typedef struct dzi2050t{
	short		weight;		/* ヘッド重量			*/
	short		status;		/* ヘッドステータス		*/
} DZI2050T;

/*------------------------------------------------------*/
/*	秤診断データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzi205t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct dzi2050t	wh_3rd[32];	/* 3段目重量&ステータス[nof_wh]	*/
	struct dzi2050t	wh_4th[32];	/* 4段目重量&ステータス[nof_bh]	*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct dzi2050t	df[8];		/* WH重量&ステータス[nof_df]	*/
	unsigned char	nof_afv;	/* ＡＦＶセル数			*/
	unsigned short	afv_out[8];	/* ＡＦＶセル出力		*/
} DZI205T;

#if	defined(STORAGE)
struct dzi205t	dzi205s;
#else
#if	!defined(NOEXTERN)
extern struct dzi205t	dzi205s;
#endif
#endif

#endif
