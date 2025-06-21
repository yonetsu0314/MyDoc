/*======================================================================
 * File name	: pri1011.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/rcs/pri1011.h 1.1 1970/01/01 00:00:00 ohtani Exp $
 *$Log: pri1011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  ohtani
 * 2007/09/12 17:59:24
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2007, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI1011_H)
#define	PRI1011_H
/*------------------------------------------------------*/
/*	計算時刻					*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri1011t {
	unsigned char	nof_ch;		/* number of channel			*/
	unsigned long	cb_caltm[4];	/* time of combinated calcuration	*/
} PRI1011T;

#if	defined(STORAGE)
struct pri1011t	pri1011s;
#else
#if	!defined(NOEXTERN)
extern struct pri1011t	pri1011s;
#endif
#endif

#endif
