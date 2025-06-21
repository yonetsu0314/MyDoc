/*======================================================================
 * File name    : nzi009.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi009.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/RCS/nzi009.h 1.1 1999/05/27 16:35:17 kawamura
 *Log: nzi009.h
 * リビジョン 1.1  1999/05/27  16:35:17  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI009_H)
#define	NZI009_H

#include	<nzcmd/nzprm.h>

/*------------------------------------------------------*/
/*	各種データの応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi009t {
	enum nzprm001e	info_no;	/* ﾃﾞｰﾀの種類		*/
	unsigned char	info_desc;	/* 識別番号		*/
	unsigned char	nof_data;	/* ﾃﾞｰﾀの数		*/
	long		data[32];	/* ﾃﾞｰﾀ			*/
} NZI009T;

#if	defined(STORAGE)
struct nzi009t	nzi009s;
#else
#if	!defined(NOEXTERN)
extern struct nzi009t	nzi009s;
#endif
#endif

#endif
