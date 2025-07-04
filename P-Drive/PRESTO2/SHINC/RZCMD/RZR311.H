/*======================================================================
 * File name    : rzr311.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	gΉvZg£p[^f[^\’ig£Rj
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr311.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr311.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * ϊrW
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr311.h 1.2 1996/07/15 13:13:16 hiroki
 *Log: rzr311.h
 * rW 1.2  1996/07/15  13:13:16  hiroki
 * nof->head π@max_scaleΖ·ιB
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR311_H)
#define	RZR311_H

/*------------------------------------------------------*/
/*	l`w@rb`kdi`s`XUdlj		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr311t {
	unsigned char	max_scale;	/* max scale       		*/
} RZR311T;

#if	defined(STORAGE)
struct rzr311t	rzr311s;
#else
#if	!defined(NOEXTERN)
extern struct rzr311t	rzr311s;
#endif
#endif

#endif
