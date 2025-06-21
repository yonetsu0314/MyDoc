/*======================================================================
 * File name    : rzr039.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	全排出停止データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr039.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr039.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr039.hv  2.1  93/11/02 15:43:12  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr039.hv
 * Revision 2.1  93/11/02 15:43:12  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR039_H)
#define	RZR039_H


/*------------------------------------------------------*/
/*	運転停止パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr039t {
	unsigned char	ch_id;		/* channel number		*/
} RZR039T;

#if	defined(STORAGE)
struct rzr039t	rzr039s;
#else
#if	!defined(NOEXTERN)
extern struct rzr039t	rzr039s;
#endif
#endif

#endif
