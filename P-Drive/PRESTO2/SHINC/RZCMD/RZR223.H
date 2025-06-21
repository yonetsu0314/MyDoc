/*======================================================================
 * File name    : rzr223.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（２）データ構造
 *	特殊アベコン（サイクル／レートの項目追加）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr223.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr223.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr223.h 1.1 1996/03/19 11:03:14 hiroki
 *Log: rzr223.h
 * リビジョン 1.1  1996/03/19  11:03:14  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR223_H)
#define	RZR223_H

/*------------------------------------------------------*/
/*	特殊アベコンパラメータパケット			*/
/*------------------------------------------------------*/

typedef struct rzr223t {
	unsigned char	avg_cycle;	/* average cycle		*/
	unsigned char 	avg_ratio;	/* average ratio		*/
} RZR223T;

#if	defined(STORAGE)
struct rzr223t	rzr223s;
#else
#if	!defined(NOEXTERN)
extern struct rzr223t	rzr223s;
#endif
#endif

#endif
