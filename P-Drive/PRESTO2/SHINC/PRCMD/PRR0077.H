/*======================================================================
 * File name	: prr0077.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡ偏差　構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0077.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0077.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *$Header: p:/include/rzcmd/rcs/rzr077.h 1.3 1996/07/12 18:19:45 hiroki Exp
 *$Log: rzr077.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0077_H)
#define	PRR0077_H

/*------------------------------------------------------*/
/*   ＡＴＡ偏差パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/
typedef struct 	prr0077t {
	unsigned char	ch_id;		/* channel number	*/
	long		ata_dev;	/* ATA 偏差     	*/
} PRR0077T;

#if	defined(STORAGE)
struct prr0077t	prr0077s;
#else
#if	!defined(NOEXTERN)
extern struct prr0077t	prr0077s;
#endif
#endif

#endif
