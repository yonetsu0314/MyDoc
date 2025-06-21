/*======================================================================
 * File name    : rzr411.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算拡張パラメータデータ構造（拡張４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr411.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr411.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr411.h 1.1 1996/10/14 20:25:12 hiroki
 *Log: rzr411.h
 * リビジョン 1.1  1996/10/14  20:25:12  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR411_H)
#define	RZR411_H

/*------------------------------------------------------*/
/*	ホッパ取り替えビットパターン　                  */
/*							*/
/*------------------------------------------------------*/

typedef struct rzr411t {
	unsigned long	wh_chg_ptn;/* WH active change head[nof_head/4]*/
} RZR411T;

#if	defined(STORAGE)
struct rzr411t	rzr411s;
#else
#if	!defined(NOEXTERN)
extern struct rzr411t	rzr411s;
#endif
#endif

#endif
