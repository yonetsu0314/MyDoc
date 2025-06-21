/*======================================================================
 * File name    : nzr025.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr025.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr025.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzr025.h 1.1 1999/05/27 16:35:57 kawamura
 *Log: nzr025.h
 * リビジョン 1.1  1999/05/27  16:35:57  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR025_H)
#define	NZR025_H

#include	<nzcmd/nzprm.h>

/*------------------------------------------------------*/
/*	各種データの要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzr025t {
	enum nzprm001e	info_no;	/* ﾃﾞｰﾀの種類		*/
	unsigned char	info_desc;	/* 識別番号		*/
} NZR025T;

#if	defined(STORAGE)
struct nzr025t	nzr025s;
#else
#if	!defined(NOEXTERN)
extern struct nzr025t	nzr025s;
#endif
#endif

#endif
