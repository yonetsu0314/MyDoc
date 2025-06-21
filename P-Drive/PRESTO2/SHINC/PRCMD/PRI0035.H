/*======================================================================
 * File name	: pri0035.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0035.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0035.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzi035.h 1.2 1994/11/04 15:20:49 ohtani Exp
 *log: rzi035.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0035_H)
#define	PRI0035_H


#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	ネットワーク参加状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0035t {
	enum prprm001e	brd_id;		/* ボードID番号			*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	net_map[32];	/* マップ格納バッファ		*/
	unsigned short	recon;		/* リコンフィグ発生回数		*/
} PRI0035T;

#if	defined(STORAGE)
struct pri0035t	pri0035s;
#else
#if	!defined(NOEXTERN)
extern struct pri0035t	pri0035s;
#endif
#endif

#endif
