/*======================================================================
 * File name    : nzr015.h
 * Original	: rzr015.h
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr015.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr015.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzr015.h 1.3 1970/01/01 00:00:00 kawamura
 *Log: nzr015.h
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2002/08/13  08:38
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR015_H)
#define	NZR015_H

enum nzr0153e {				/* フィーダの種類		*/
	FEEDER_RF,			/* ＲＦ				*/
	FEEDER_DF,			/* ＤＦ				*/
	FEEDER_CF,			/* ＣＦ				*/
	FEEDER_AF,			/* AF(assistant feeder)		*/
};

enum nzr0150e {				/* 位相制御			*/
	PHASE_HALF,			/* 半波				*/
	PHASE_ALL,			/* 全波				*/
};

enum nzr0151e {				/* 光電子ＳＷ停止		*/
	OPTSW_OFF,			/* なし				*/
	OPTSW_LLVL,			/* ＬＯＷレベル			*/
	OPTSW_HLVL = 3,			/* ＨＩＧＨレベル		*/
	OPTSW_LEDGE = 5,		/* ＬＯＷレベル			*/
	OPTSW_HEDGE = 7,		/* ＨＩＧＨレベル		*/
};

enum nzr0152e {				/* シャッタ			*/
	SHUTR_OFF,			/* なし				*/
	SHUTR_LOW,			/* ＬＯＷ			*/
	SHUTR_HIGH = 3,			/* ＨＩＧＨ			*/
};

enum nzr0154e {				/* スロースタート		*/
	SLOW_START_OFF,			/* しない			*/
	SLOW_START_ON,			/* する				*/
};

/*------------------------------------------------------*/
/*	フィーダ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzr015t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	fd_assign_ptn;	/* Hopper assign pattern
					   [nof_head/4]			*/
	enum nzr0153e	feeder_id;	/* feeder id			*/
	unsigned char	min_amp;	/* minimun aplitude		*/
	unsigned char	max_amp;	/* maximun aplitude		*/
	unsigned char	multiplier;	/* フィーダ時間倍率		*/
	enum nzr0150e	phase_type;	/* 位相				*/
	enum nzr0151e	opt_sw_ctrl;	/* 光電子SWによる停止モード	*/
	enum nzr0152e	shutter_ctrl;	/* シャッタ連動モード		*/
	unsigned char	shutter_off_delay;/* シャッタ停止遅れ時間	*/
	enum nzr0154e	slow_start;	/* スロースタート		*/
} NZR015T;

#if	defined(STORAGE)
struct nzr015t	nzr015s;
#else
#if	!defined(NOEXTERN)
extern struct nzr015t	nzr015s;
#endif
#endif

#endif
