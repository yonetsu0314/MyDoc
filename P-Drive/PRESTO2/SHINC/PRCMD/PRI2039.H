/*======================================================================
 * File name	: pri2039.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター状態データ構造
 *	ステイル・ロックタイム・テキスト（ATA98）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri2039.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri2039.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi239.h 1.2 1999/12/17 14:35:01 hiroe Exp
 *log: rzi239.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI2039_H)
#define	PRI2039_H

enum pri20390e {		/* STALE MODE (現在の状態）*/
	STALE98_TIMER_READY,	/*STALE TIMER 停止している*/
	STALE98_TIMER_START,	/*STALE TIMER 動作している*/
	STALE98_TIMEUP,		/*STALE TIMER タイムアップしている*/
};

enum pri20391e {		/* STALE DRAIN MODE (現在の状態）*/
	STALE98_DRAIN_READY,	/*DRAIN TIMER 停止している*/
	STALE98_DRAIN_START,	/*DRAIN TIMER 動作している*/
	STALE98_EMPTY_WAIT,	/*WHの空待ち状態*/
	STALE98_DRAIN_FINAL,	/*STALE DRAIN DRY CYCLE 実行中*/
	STALE98_DRAIN_COMPLETE,	/*STALE DRAINは終了した*/
};

/*------------------------------------------------------*/
/*	ステイル・ロックタイム・テキスト		*/
/*	＜ＡＴＡ９８仕様＞				*/
/*------------------------------------------------------*/

typedef struct pri2039t {
	unsigned short	stale98_time;	/* ステイル・ロック・タイム     */
	enum pri20390e	stale98_mode;	/* 現在の状態                   */
	unsigned short	stale98_tmr;	/* ステイルタイマ、ドレインタイマ*/
	unsigned char	stale98_dcnt;	/* ダウンカウント値		*/
	unsigned char	stale98_SDEnable;	 /* ステイルドレインENABLE */
	enum pri20391e	stale98_drain_mode;/* 現在の状態             */
	unsigned short	stale98_drain_tmr; /* ステイルドレインタイマ */

} PRI2039T;

#if	defined(STORAGE)
struct pri2039t	pri2039s;
#else
#if	!defined(NOEXTERN)
extern struct pri2039t	pri2039s;
#endif
#endif

#endif
