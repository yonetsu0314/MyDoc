/*======================================================================
 * File name    : rzr045.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	手動計算データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr045.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr045.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr045.hv  2.1  93/11/02 15:43:32  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr045.hv
 * Revision 2.1  93/11/02 15:43:32  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR045_H)
#define	RZR045_H


/*------------------------------------------------------*/
/*	テスト組み合わせ計算パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr045t {
	unsigned char	ch_id;		/* channel number		*/
} RZR045T;

#if	defined(STORAGE)
struct rzr045t	rzr045s;
#else
#if	!defined(NOEXTERN)
extern struct rzr045t	rzr045s;
#endif
#endif

#endif
