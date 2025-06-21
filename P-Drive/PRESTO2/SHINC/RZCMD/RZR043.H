/*======================================================================
 * File name    : rzr043.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	全排出停止データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr043.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr043.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr043.hv  2.1  93/11/02 15:43:36  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr043.hv
 * Revision 2.1  93/11/02 15:43:36  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR043_H)
#define	RZR043_H


/*------------------------------------------------------*/
/*	全排出停止パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr043t {
	unsigned char	ch_id;		/* channel number		*/
} RZR043T;

#if	defined(STORAGE)
struct rzr043t	rzr043s;
#else
#if	!defined(NOEXTERN)
extern struct rzr043t	rzr043s;
#endif
#endif

#endif
