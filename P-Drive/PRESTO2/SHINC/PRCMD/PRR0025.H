/*======================================================================
 * File name	: prr0025.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0025.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0025.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr025a.h 1.2 1995/11/16 16:13:23 hiroe Exp
 *log: rzr025a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0025_H)
#define	PRR0025_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	プログラム番号の要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0025t {
	enum prprm001e	brd_id;		/* デバイスID番号		*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	node_id;	/* ノード番号			*/
} PRR0025T;

#if	defined(STORAGE)
struct prr0025t	prr0025s;
#else
#if	!defined(NOEXTERN)
extern struct prr0025t	prr0025s;
#endif
#endif

#endif
