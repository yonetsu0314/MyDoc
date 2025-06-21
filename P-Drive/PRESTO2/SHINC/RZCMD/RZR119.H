/*======================================================================
 * File name    : rzr119.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	４－２０ｍＡ出力に対応するレベル重量スパン
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr119.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr119.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzr119.h 1.4 1994/09/12 14:16:08 hiroe
 *Log: rzr119.h
 * リビジョン 1.4  1994/09/12  14:16:08  hiroe
 * 上下限偏差を上下限重量にする。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR119_H)
#define	RZR119_H


typedef struct rzr1191t {
	unsigned short	targ;		/* 目標重量			*/
	unsigned short	upper;		/* 上限重量			*/
	unsigned short	lower;		/* 下限重量			*/
} RZR1191T;

/*------------------------------------------------------*/
/*	　　スパン重量パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr119t {
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するCHのDF数		*/
	struct rzr1191t	spanwt[8];	/* DF paramter[nof_df]		*/
} RZR119T;

#if	defined(STORAGE)
struct rzr119t	rzr119s;
#else
#if	!defined(NOEXTERN)
extern struct rzr119t	rzr119s;
#endif
#endif

#endif
