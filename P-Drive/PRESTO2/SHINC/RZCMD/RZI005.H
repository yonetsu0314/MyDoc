/*======================================================================
 * File name    : rzi005.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	単重データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi005.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi005.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi005.hv  2.1  93/11/02 15:40:32  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi005.hv
 * Revision 2.1  93/11/02 15:40:32  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI005_H)
#define	RZI005_H


/*------------------------------------------------------*/
/*	単重更新結果データのパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi005t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	unsigned short	weight[8];	/* 単重				*/
} RZI005T;

#if	defined(STORAGE)
struct rzi005t	rzi005s;
#else
#if	!defined(NOEXTERN)
extern struct rzi005t	rzi005s;
#endif
#endif

#endif
