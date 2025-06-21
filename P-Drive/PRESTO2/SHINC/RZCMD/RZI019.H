/*======================================================================
 * File name    : rzi019.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	受信完了データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi019.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi019.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi019.hv  2.2  93/12/26 16:12:08  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi019.hv
 * Revision 2.2  93/12/26 16:12:08  ryoji
 * 受信完了にチャンネル番号付加する
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI019_H)
#define	RZI019_H


/*------------------------------------------------------*/
/*	受信完了のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi019t {
	unsigned char	command;	/* 受信コマンド			*/
	unsigned char	ch_id;		/* c1c2				*/
} RZI019T;

#if	defined(STORAGE)
struct rzi019t	rzi019s;
#else
#if	!defined(NOEXTERN)
extern struct rzi019t	rzi019s;
#endif
#endif

#endif
