/*======================================================================
 * File name    : prr0083.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（要求）仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0083.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0083.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shinc/rzcmd/RCS/rzr083.h 1.1 1970/01/01
 *Log: rzr083.h
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR0083_H)
#define	PRR0083_H

typedef struct 	prr0083t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	req_type;	/* reqest type   		*/
	unsigned char	data_page;	/* data page          		*/
} PRR0083T;

#if	defined(STORAGE)
struct prr0083t	prr0083s;
#else
#if	!defined(NOEXTERN)
extern struct prr0083t	prr0083s;
#endif
#endif

#endif
