/*======================================================================
 * File name    : rzr089.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動パラメータ1リクエスト構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr089.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr089.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr089.h 1.1 1997/11/19 16:45:19 itami
 *Log: rzr089.h
 * リビジョン 1.1  1997/11/19  16:45:19  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR089_H)
#define	RZR089_H


/*------------------------------------------------------*/
/*	マルチ連動パラメータ1リクエスト構造		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr089t {
	unsigned char	ch_id;			/* channel number	*/
} RZR089T;

#if	defined(STORAGE)
struct rzr089t	rzr089s;
#else
#if	!defined(NOEXTERN)
extern struct rzr089t	rzr089s;
#endif
#endif

#endif
