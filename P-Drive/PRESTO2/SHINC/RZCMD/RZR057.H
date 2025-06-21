/*======================================================================
 * File name    : rzr057.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr057.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr057.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr057.hv  1.1  93/11/02 17:16:20  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr057.hv
 * Revision 1.1  93/11/02 17:16:20  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR057_H)
#define	RZR057_H


typedef struct rzr0570t{
	unsigned char	ctrl_mode;	/* AFDモード			*/
	unsigned short	ctrl_sw;	/* AFD動作ＳＷ			*/
	unsigned short	start_delay;	/* 制御開始遅れサイクル数	*/
	unsigned char	df_rf_ratio;	/* DF-RF振幅比			*/
	unsigned char	target_head;	/* 目標選択ヘッド数		*/
	unsigned char	amp_center;	/* RF振幅制御中心値		*/
	unsigned char	amp_max;	/* 振幅絶対上限			*/
	unsigned char	amp_min;	/* 振幅絶対下限			*/
	unsigned char	amp_range;	/* 振幅制御範囲			*/
} RZR0570T;

/*------------------------------------------------------*/
/*	ＡＦＤヘッドデータパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr057t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr0570t	sect[8];	/* AFD paramter[nof_sect]	*/
} RZR057T;

#if	defined(STORAGE)
struct rzr057t	rzr057s;
#else
#if	!defined(NOEXTERN)
extern struct rzr057t	rzr057s;
#endif
#endif

#endif
