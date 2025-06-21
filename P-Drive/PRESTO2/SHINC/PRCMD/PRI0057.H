/*======================================================================
 * File name	: pri0057.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0057.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0057.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0057_H)
#define	PRI0057_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	ホッパ駆動回数応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0057t {
	enum prprm004e	hopper_id;	/* ホッパ識別フラグ	*/
	unsigned char	nof_hopper;	/* ホッパ数		*/
	unsigned long	nof_times[32];	/* ホッパ駆動回数	*/
} PRI0057T;

#if	defined(STORAGE)
struct pri0057t	pri0057s;
#else
#if	!defined(NOEXTERN)
extern struct pri0057t	pri0057s;
#endif
#endif

#endif
