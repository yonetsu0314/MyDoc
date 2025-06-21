/*======================================================================
 * File name    : rzi139.h
 * Original	: rzi039.h
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター状態データ構造
 *	（拡張１）ステイル・ロックタイム・テキスト	
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi139.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi139.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi139.h 1.2 1998/04/01 15:50:18 hiroki
 *Log: rzi139.h
 * リビジョン 1.2  1998/04/01  15:50:18  hiroki
 * 変数名修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI139_H)
#define	RZI139_H

enum rzi1390e {			/* STALE MODE (現在の状態）*/
	STALE_TIMER_STOP,	/*STALE TIMER 停止している*/
	STALE_TIMER_START,	/*STALE TIMER 動作している*/
	STALE_LOCK_OUT,		/*STALE LOCK OUTとなった*/
	STALE_DRAIN,		/*STALE DRAINが開始された*/
	STALE_EMPTY_WAIT,	/*STALE DRAINﾀｲﾏｰがﾀｲﾑｱｯﾌﾟし、ＷＨ空状態をチェック中*/
	STALE_DRAIN_FINAL,	/*全ヘッドのｗｈの空が確認され最終ドレインの動作中*/
};

/*------------------------------------------------------*/
/*	（拡張１）ステイル・ロックタイム・テキスト	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi139t {
	unsigned int	stale_time;	/* ステイル・ロック・タイム     */
	enum rzi1390e	stale_mode;	/* 現在の状態                   */
	unsigned int	stale_tmr;	/* ステイルタイマ、ドレインタイマ*/
	unsigned char	stale_dcnt;	/* ダウンカウント値		*/
} RZI139T;

#if	defined(STORAGE)
struct rzi139t	rzi139s;
#else
#if	!defined(NOEXTERN)
extern struct rzi139t	rzi139s;
#endif
#endif

#endif
