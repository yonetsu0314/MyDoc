/*======================================================================
 * File name    : dzr203.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ調整開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzr203.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr203.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr203.hv  1.1  94/05/19 09:10:12  sato
 *Log: p:/include/dzcmd/rcs/dzr203.hv
 * Revision 1.1  94/05/19 09:10:12  sato
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZR203_H)
#define	DZR203_H

/*------------------------------------------------------*/
/*	AFV調整パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzr203t {
	unsigned char	ch_id;		/* channel number		*/
	int		adj_mode;	/* adjust mode			*/
} DZR203T;

#if	defined(STORAGE)
struct dzr203t	dzr203s;
#else
#if	!defined(NOEXTERN)
extern struct dzr203t	dzr203s;
#endif
#endif

#endif
