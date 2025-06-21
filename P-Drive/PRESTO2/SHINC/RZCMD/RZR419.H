/*======================================================================
 * File name	: rzr419.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ供給量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr419.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr419.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr419.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr419.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(RZR419_H)
#define	RZR419_H

typedef struct rzr4190t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} RZR4190T;

/*------------------------------------------------------*/
/*	補助フィーダ供給量パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr419t {
	unsigned char	min_af_no;	/* 最小AF番号			*/
	unsigned char	nof_af;		/* 該当するCHのAF数		*/
	struct rzr4190t	af[32];		/* AF paramter[nof_af]		*/
} RZR419T;

#if	defined(STORAGE)
struct rzr419t	rzr419s;
#else
#if	!defined(NOEXTERN)
extern struct rzr419t	rzr419s;
#endif
#endif

#endif
