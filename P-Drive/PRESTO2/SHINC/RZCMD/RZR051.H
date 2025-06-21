/*======================================================================
 * File name    : rzr051.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr051.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr051.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr051.hv  2.1  93/11/02 15:43:44  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr051.hv
 * Revision 2.1  93/11/02 15:43:44  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR051_H)
#define	RZR051_H


enum rzr0510e {				/* テスト駆動			*/
	TDRV_NRML,			/* 通常（連続）			*/
	TDRV_SGNL,			/* 一回動作			*/
};

/*------------------------------------------------------*/
/*	テスト駆動開始データのパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr051t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned long	rf_ptn;		/* RF pattern			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	wh2_ptn;	/* WH2 pattern(BH side)		*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned char	rs_ptn;		/* RS pattern			*/
	unsigned char	dth_ptn;	/* DTH pattern			*/
	unsigned char	th_ptn;		/* TH pattern			*/
	enum rzr0510e	drv_cyc;	/* Drive cycle			*/
} RZR051T;

#if	defined(STORAGE)
struct rzr051t	rzr051s;
#else
#if	!defined(NOEXTERN)
extern struct rzr051t	rzr051s;
#endif
#endif

#endif
