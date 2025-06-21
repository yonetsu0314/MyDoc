/*======================================================================
 * File name	: prr0105.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データパラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0105.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0105.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr019.h 1.1 1999/05/27 16:33:47 kawamura Exp
 *log: nzr019.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0105_H)
#define	PRR0105_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

enum prr01050e {			/* 重量データのタイプ		*/
	AUTO_SEL,			/* 自動切り替え			*/
	NO_3RD,				/* ３段目データ			*/
	NO_4TH,				/* ４段目データ			*/
	NO_5TH,				/* ５段目データ			*/
};

/*------------------------------------------------------*/
/*	重量波形データの要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0105t {
	unsigned char	wh_no;		/* WH番号		*/
	enum prr01050e	type;		/* 重量データのタイプ	*/
	enum prprm004e	hopper_id;	/* WH/PH識別フラグ	*/
} PRR0105T;

#if	defined(STORAGE)
struct prr0105t	prr0105s;
#else
#if	!defined(NOEXTERN)
extern struct prr0105t	prr0105s;
#endif
#endif

#endif
