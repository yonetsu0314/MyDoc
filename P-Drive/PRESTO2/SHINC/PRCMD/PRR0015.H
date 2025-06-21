/*======================================================================
 * File name	: prr0015.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/rcs/prr0015.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0015.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2006/09/25
 * ストップパターンオートを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr015.h 1.3 1970/01/01 00:00:00 kawamura Exp
 *log: nzr015.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0015_H)
#define	PRR0015_H

enum prr00153e {			/* フィーダの種類		*/
	FEEDER_RF,			/* ＲＦ				*/
	FEEDER_DF,			/* ＤＦ				*/
	FEEDER_CF,			/* ＣＦ				*/
	FEEDER_AF,			/* AF(assistant feeder)		*/
};

enum prr00150e {			/* 位相制御			*/
	PHASE_HALF,			/* 半波				*/
	PHASE_ALL,			/* 全波				*/
};

enum prr00151e {			/* 光電子ＳＷ停止		*/
	OPTSW_OFF,			/* なし				*/
	OPTSW_LLVL,			/* ＬＯＷレベル			*/
	OPTSW_HLVL = 3,			/* ＨＩＧＨレベル		*/
	OPTSW_LEDGE = 5,		/* ＬＯＷレベル			*/
	OPTSW_HEDGE = 7,		/* ＨＩＧＨレベル		*/
};

enum prr00152e {			/* シャッタ			*/
	SHUTR_OFF,			/* なし				*/
	SHUTR_LOW,			/* ＬＯＷ			*/
	SHUTR_HIGH = 3,			/* ＨＩＧＨ			*/
};

enum prr00154e {			/* スロースタート		*/
	SLOW_START_OFF,			/* しない			*/
	SLOW_START_ON,			/* する				*/
};

enum prr00155e {			/* ストップパターン生成		*/
	STOP_PTN_OFF,			/* しない			*/
	STOP_PTN_ON,			/* する				*/
	STOP_PTN_AUTO,			/* オート 時間25以下はオフ	*/
};

/*------------------------------------------------------*/
/*	フィーダ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0015t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	fd_assign_ptn;	/* feeder assign pattern	*/
	enum prr00153e	feeder_id;	/* feeder id			*/
	unsigned char	min_amp;	/* minimun aplitude		*/
	unsigned char	max_amp;	/* maximun aplitude		*/
	unsigned char	multiplier;	/* フィーダ時間倍率		*/
	enum prr00150e	phase_type;	/* 位相				*/
	enum prr00151e	opt_sw_ctrl;	/* 光電子SWによる停止モード	*/
	enum prr00152e	shutter_ctrl;	/* シャッタ連動モード		*/
	unsigned char	shutter_off_delay;/* シャッタ停止遅れ時間	*/
	enum prr00154e	slow_start;	/* スロースタート		*/
	enum prr00155e	stop_ptn;	/* ストップパターン生成		*/
} PRR0015T;

#if	defined(STORAGE)
struct prr0015t	prr0015s;
#else
#if	!defined(NOEXTERN)
extern struct prr0015t	prr0015s;
#endif
#endif

#endif
