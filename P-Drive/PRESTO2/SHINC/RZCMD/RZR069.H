/*======================================================================
 * File name    : rzr069.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	サンプル排出仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr069.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr069.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzr069.h 1.2 1994/09/25 08:39:29 hiroe
 *Log: rzr069.h
 * リビジョン 1.2  1994/09/25  08:39:29  hiroe
 * エラー修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR069_H)
#define	RZR069_H

typedef struct 	rzr069t {
	unsigned char	ch_id;			/* channel number	*/
	unsigned long	sect_assign_ptn;	/* section pattern	*/
} RZR069T;

#if	defined(STORAGE)
struct rzr069t	rzr069s;
#else
#if	!defined(NOEXTERN)
extern struct rzr069t	rzr069s;
#endif
#endif

#endif
