/*======================================================================
 * File name    : rzr055.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤヘッドデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr055.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr055.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr055.hv  1.1  93/11/02 19:32:50  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr055.hv
 * Revision 1.1  93/11/02 19:32:50  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR055_H)
#define	RZR055_H


typedef struct rzr0550t{
	short		trg;		/* 目標重量			*/
	short		std;		/* 標準偏差			*/
} RZR0550T;

/*------------------------------------------------------*/
/*	ＡＦＤヘッドデータパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr055t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct rzr0550t	wh[32];		/* 目標重量＆標準偏差[nof_wh]	*/
} RZR055T;

#if	defined(STORAGE)
struct rzr055t	rzr055s;
#else
#if	!defined(NOEXTERN)
extern struct rzr055t	rzr055s;
#endif
#endif

#endif
