/*======================================================================
 * File name    : rzr083.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（要求）仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr083.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr083.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr083.h 1.1 1996/12/19 09:09:20 hiroki
 *Log: rzr083.h
 * リビジョン 1.1  1996/12/19  09:09:20  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR083_H)
#define	RZR083_H

typedef struct 	rzr083t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	req_type;	/* reqest type   		*/
	unsigned char	data_page;	/* data page          		*/
} RZR083T;

#if	defined(STORAGE)
struct rzr083t	rzr083s;
#else
#if	!defined(NOEXTERN)
extern struct rzr083t	rzr083s;
#endif
#endif

#endif
