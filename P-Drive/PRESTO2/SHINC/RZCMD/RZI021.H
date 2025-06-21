/*======================================================================
 * File name    : rzi021.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	動作完了データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi021.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi021.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi021.hv  2.1  93/11/02 15:40:54  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi021.hv
 * Revision 2.1  93/11/02 15:40:54  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI021_H)
#define	RZI021_H


/*------------------------------------------------------*/
/*	動作完了のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi021t {
	unsigned char	command;	/* 受信コマンド			*/
	unsigned char	ch_id;		/* c1c2				*/
} RZI021T;

#if	defined(STORAGE)
struct rzi021t	rzi021s;
#else
#if	!defined(NOEXTERN)
extern struct rzi021t	rzi021s;
#endif
#endif

#endif
