/*======================================================================
 * File name	: prr0125.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0125.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0125.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0125_H)
#define	PRR0125_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	ホッパ駆動回数要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0125t {
	enum prprm004e	hopper_id;	/* ホッパ識別フラグ	*/
} PRR0125T;

#if	defined(STORAGE)
struct prr0125t	prr0125s;
#else
#if	!defined(NOEXTERN)
extern struct prr0125t	prr0125s;
#endif
#endif

#endif
