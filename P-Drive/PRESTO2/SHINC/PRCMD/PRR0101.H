/*======================================================================
 * File name	: prr0101.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0101.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0101.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr025.h 1.1 1999/05/27 16:35:57 kawamura Exp
 *log: nzr025.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0101_H)
#define	PRR0101_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	各種データの要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0101t {
	enum prprm002e	info_no;	/* ﾃﾞｰﾀの種類		*/
	unsigned char	info_desc;	/* 識別番号		*/
} PRR0101T;

#if	defined(STORAGE)
struct prr0101t	prr0101s;
#else
#if	!defined(NOEXTERN)
extern struct prr0101t	prr0101s;
#endif
#endif

#endif
