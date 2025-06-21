/*======================================================================
 * File name    : rzr151.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張テスト駆動開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr151.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr151.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr151.h 1.1 1970/01/01 00:00:00 itami
 *Log: rzr151.h
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR151_H)
#define	RZR151_H

/*------------------------------------------------------*/
/*	拡張テスト駆動開始データのパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr151t {
	unsigned long	sht_ptn;	/* SHUTTER pattern		*/
	unsigned char	sht_flg;	/* SHUTTER ctrl flag		*/
	unsigned long	spa_ptn;	/* spare pattern		*/
	unsigned char	spa_flg;	/* ctrl flag			*/
} RZR151T;

#if	defined(STORAGE)
struct rzr151t	rzr151s;
#else
#if	!defined(NOEXTERN)
extern struct rzr151t	rzr151s;
#endif
#endif

#endif
