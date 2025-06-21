/*======================================================================
 * File name	: prr0127.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数設定データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0127.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0127.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0127_H)
#define	PRR0127_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	ホッパ駆動回数設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0127t {
	enum prprm004e	hopper_id;	/* ホッパ識別フラグ	*/
	unsigned char	hopper_no;	/* ホッパ番号		*/
	unsigned long	nof_times;	/* ホッパ駆動回数	*/
} PRR0127T;

#if	defined(STORAGE)
struct prr0127t	prr0127s;
#else
#if	!defined(NOEXTERN)
extern struct prr0127t	prr0127s;
#endif
#endif

#endif
