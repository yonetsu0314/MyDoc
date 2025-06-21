/*======================================================================
 * File name    : rzr081.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイマーコントロールデータの構造体
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr081.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr081.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr081.h 1.2 1996/09/19 15:32:59 hiroki
 *Log: rzr081.h
 * リビジョン 1.2  1996/09/19  15:32:59  hiroki
 * チャンネルＩＤを追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR081_H)
#define	RZR081_H

typedef struct rzr0811t {
	unsigned char	port;		/* ポート番号　　　		*/
	unsigned char	init;		/* 初期時間設定			*/
	unsigned char	off;		/* ＯＦＦ時間設定		*/
	unsigned char	on;		/* ＯＮ時間設定			*/
} RZR0811T;
/*------------------------------------------------------*/
/*	タイマ設定パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr081t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_port;	/* 該当するポート数		*/
	struct rzr0811t	timer[16];	/* timer paramter[nof_port]	*/
} RZR081T;

#if	defined(STORAGE)
struct rzr081t	rzr081s;
#else
#if	!defined(NOEXTERN)
extern struct rzr081t	rzr081s;
#endif
#endif

#endif
