/*======================================================================
 * File name    : rzr209.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張計量特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr209.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr209.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr209.h 1.2 1970/01/01 00:00:00 itami
 *Log: rzr209.h
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 * 2000/12/12 自動零点間隔追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR209_H)
#define	RZR209_H

/*------------------------------------------------------*/
/*	拡張秤計量特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr209t {
	unsigned short	stable_time;	/* stable time constant		*/
	unsigned short	empty_weight;	/* empty weight			*/
	unsigned char	zero_interval;	/* auto zero interval		*/
} RZR209T;

#if	defined(STORAGE)
struct rzr209t	rzr209s;
#else
#if	!defined(NOEXTERN)
extern struct rzr209t	rzr209s;
#endif
#endif

#endif
