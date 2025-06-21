/*======================================================================
 * File name    : rzr035.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン調整データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr035.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr035.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr035.hv  2.1  93/11/02 15:42:16  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr035.hv
 * Revision 2.1  93/11/02 15:42:16  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR035_H)
#define	RZR035_H


/*------------------------------------------------------*/
/*	スパン校正パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr035t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	adj_head_ptn;	/* 調整ヘッドビットパターン	*/
} RZR035T;

#if	defined(STORAGE)
struct rzr035t	rzr035s;
#else
#if	!defined(NOEXTERN)
extern struct rzr035t	rzr035s;
#endif
#endif

#endif
