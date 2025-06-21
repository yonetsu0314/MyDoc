/*======================================================================
 * File name	: prr0071.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0071.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0071.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzr071.h 1.1 1994/11/04 15:17:17 ohtani Exp
 *log: rzr071.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0071_H)
#define	PRR0071_H


#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	ネットワーク参加状態の要求パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0071t {
	enum prprm001e	brd_id;		/* ボードID番号			*/
	unsigned char	dev_no;		/* デバイス番号			*/
} PRR0071T;

#if	defined(STORAGE)
struct prr0071t	prr0071s;
#else
#if	!defined(NOEXTERN)
extern struct prr0071t	prr0071s;
#endif
#endif

#endif
