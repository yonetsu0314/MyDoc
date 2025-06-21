/*======================================================================
 * File name    : rzi035.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi035.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi035.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi035.h 1.2 1994/11/04 15:20:49 ohtani
 *Log: rzi035.h
 * リビジョン 1.2  1994/11/04  15:20:49  ohtani
 * マップ格納バッファをビット・マップ化する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI035_H)
#define	RZI035_H


#include	<rzcmd/rzprm.h>

/*------------------------------------------------------*/
/*	ネットワーク参加状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi035t {
	enum rzprm001e	brd_id;		/* ボードID番号			*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	net_map[32];	/* マップ格納バッファ		*/
	unsigned int	recon;		/* リコンフィグ発生回数		*/
} RZI035T;

#if	defined(STORAGE)
struct rzi035t	rzi035s;
#else
#if	!defined(NOEXTERN)
extern struct rzi035t	rzi035s;
#endif
#endif

#endif
