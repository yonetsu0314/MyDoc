/*======================================================================
 * File name    : rzi239.h
 * Original	: rzi139.h
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター状態データ構造
 *	（拡張２）ステイル・ロックタイム・テキスト（ATA98）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi239.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi239.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi239.h 1.2 1999/12/17 14:35:01 hiroe
 *Log: rzi239.h
 * リビジョン 1.2  1999/12/17  14:35:01  hiroe
 * enum rzi1391e -> enum rzi2391e
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZ239_H)
#define	RZI239_H

enum rzi2390e {			/* STALE MODE (現在の状態）*/
	STALE98_TIMER_READY,	/*STALE TIMER 停止している*/
	STALE98_TIMER_START,	/*STALE TIMER 動作している*/
	STALE98_TIMEUP,		/*STALE TIMER タイムアップしている*/
};

enum rzi2391e {			/* STALE DRAIN MODE (現在の状態）*/
	STALE98_DRAIN_READY,	/*DRAIN TIMER 停止している*/
	STALE98_DRAIN_START,	/*DRAIN TIMER 動作している*/
	STALE98_EMPTY_WAIT,	/*WHの空待ち状態*/
	STALE98_DRAIN_FINAL,	/*STALE DRAIN DRY CYCLE 実行中*/
	STALE98_DRAIN_COMPLETE,	/*STALE DRAINは終了した*/
};

/*------------------------------------------------------*/
/*	（拡張２）ステイル・ロックタイム・テキスト	*/
/*	＜ＡＴＡ９８仕様＞				*/
/*------------------------------------------------------*/

typedef struct rzi239t {
	unsigned int	stale98_time;	/* ステイル・ロック・タイム     */
	enum rzi2390e	stale98_mode;	/* 現在の状態                   */
	unsigned int	stale98_tmr;	/* ステイルタイマ、ドレインタイマ*/
	unsigned char	stale98_dcnt;	/* ダウンカウント値		*/
	unsigned char	stale98_SDEnable;	 /* ステイルドレインENABLE */
	enum rzi2391e	stale98_drain_mode;/* 現在の状態             */
	unsigned int	stale98_drain_tmr; /* ステイルドレインタイマ */

} RZI239T;

#if	defined(STORAGE)
struct rzi239t	rzi239s;
#else
#if	!defined(NOEXTERN)
extern struct rzi239t	rzi239s;
#endif
#endif

#endif
