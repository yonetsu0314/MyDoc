/*======================================================================
 * File name    : rzr077.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡ偏差　構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr077.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr077.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr077.h 1.3 1996/07/12 18:19:45 hiroki
 *Log: rzr077.h
 * リビジョン 1.3  1996/07/12  18:19:45  hiroki
 * ATA96仕様
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR077_H)
#define	RZR077_H

/*------------------------------------------------------*/
/*   ＡＴＡ偏差パラメータパケット   　　　		*/
/*							*/
/*------------------------------------------------------*/
typedef struct 	rzr077t {
	unsigned 	char	ch_id;		/* channel number	*/
	long 		int 	ata_dev;	/* ATA 偏差     	*/
} RZR077T;

#if	defined(STORAGE)
struct rzr077t	rzr077s;
#else
#if	!defined(NOEXTERN)
extern struct rzr077t	rzr077s;
#endif
#endif

#endif
