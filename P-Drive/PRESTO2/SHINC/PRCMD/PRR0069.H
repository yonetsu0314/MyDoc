/*======================================================================
 * File name	: prr0069.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	サンプル排出仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0069.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0069.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzr069.h 1.2 1994/09/25 08:39:29 hiroe Exp
 *log: rzr069.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0069_H)
#define	PRR0069_H

typedef struct	prr0069t {
	unsigned char	ch_id;			/* channel number	*/
	unsigned char	sect_assign_ptn;	/* section pattern	*/
} PRR0069T;

#if	defined(STORAGE)
struct prr0069t	prr0069s;
#else
#if	!defined(NOEXTERN)
extern struct prr0069t	prr0069s;
#endif
#endif

#endif
