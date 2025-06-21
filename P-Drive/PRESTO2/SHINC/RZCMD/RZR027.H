/*======================================================================
 * File name    : rzr027.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤データ要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr027.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr027.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr027.hv  2.1  93/11/02 15:42:24  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr027.hv
 * Revision 2.1  93/11/02 15:42:24  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR027_H)
#define	RZR027_H


/*------------------------------------------------------*/
/*	秤データの要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr027t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	head_no;	/* ヘッド番号			*/
} RZR027T;

#if	defined(STORAGE)
struct rzr027t	rzr027s;
#else
extern struct rzr027t	rzr027s;
#if	!defined(NOEXTERN)
#endif
#endif

#endif
