/*======================================================================
 * File name	: prr0055.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ調整開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0055.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0055.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: dzr203.hv  1.1  94/05/19 09:10:12  sato  Exp
 *log: p:/include/dzcmd/rcs/dzr203.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0055_H)
#define	PRR0055_H

/*------------------------------------------------------*/
/*	AFV調整パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0055t {
	unsigned char	ch_id;		/* channel number		*/
	short		adj_mode;	/* adjust mode			*/
} PRR0055T;

#if	defined(STORAGE)
struct prr0055t	prr0055s;
#else
#if	!defined(NOEXTERN)
extern struct prr0055t	prr0055s;
#endif
#endif

#endif
