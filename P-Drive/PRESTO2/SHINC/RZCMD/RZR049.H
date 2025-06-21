/*======================================================================
 * File name    : rzr049.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	エラー解除データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr049.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr049.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr049.hv  2.1  93/11/02 15:43:22  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr049.hv
 * Revision 2.1  93/11/02 15:43:22  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR049_H)
#define	RZR049_H


/*------------------------------------------------------*/
/*	エラー解除のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr049t {
	unsigned char	ch_id;		/* channel number		*/
} RZR049T;

#if	defined(STORAGE)
struct rzr049t	rzr049s;
#else
#if	!defined(NOEXTERN)
extern struct rzr049t	rzr049s;
#endif
#endif

#endif
