/*======================================================================
 * File name	: pri0025.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0025.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0025.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/RCS/nzi009.h 1.1 1999/05/27 16:35:17 kawamura Exp
 *log: nzi009.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0025_H)
#define	PRI0025_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	各種データの応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0025t {
	enum prprm002e	info_no;	/* ﾃﾞｰﾀの種類		*/
	unsigned char	info_desc;	/* 識別番号		*/
	unsigned char	nof_data;	/* ﾃﾞｰﾀの数		*/
	long		data[32];	/* ﾃﾞｰﾀ			*/
} PRI0025T;

#if	defined(STORAGE)
struct pri0025t	pri0025s;
#else
#if	!defined(NOEXTERN)
extern struct pri0025t	pri0025s;
#endif
#endif

#endif
