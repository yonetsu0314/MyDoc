/*======================================================================
 * File name    : rzr015.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr015.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr015.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr015.h 2.2 1970/01/01 00:00:00 kawamura
 *Log: rzr015.h
 * リビジョン 2.2  1970/01/01  00:00:00  kawamura
 * 2002/07/26  11:36
 * enumに補助フィーダ(FEEDER_AF)を追加。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR015_H)
#define	RZR015_H

enum rzr0153e {				/* フィーダの種類		*/
	FEEDER_RF,			/* ＲＦ				*/
	FEEDER_DF,			/* ＤＦ				*/
	FEEDER_CF,			/* ＣＦ				*/
	FEEDER_AF,			/* AF(assistant feeder)		*/
};

enum rzr0150e {				/* 位相制御			*/
	PHASE_HALF,			/* 半波				*/
	PHASE_ALL,			/* 全波				*/
};

enum rzr0151e {				/* 光電子ＳＷ停止		*/
	OPTSW_OFF,			/* なし				*/
	OPTSW_LLVL,			/* ＬＯＷレベル			*/
	OPTSW_HLVL = 3,			/* ＨＩＧＨレベル		*/
	OPTSW_LEDGE = 5,		/* ＬＯＷレベル			*/
	OPTSW_HEDGE = 7,		/* ＨＩＧＨレベル		*/
};

enum rzr0152e {				/* シャッタ			*/
	SHUTR_OFF,			/* なし				*/
	SHUTR_LOW,			/* ＬＯＷ			*/
	SHUTR_HIGH = 3,			/* ＨＩＧＨ			*/
};

/*------------------------------------------------------*/
/*	フィーダ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr015t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	fd_assign_ptn;	/* Hopper assign pattern
					   [nof_head/4]			*/
	enum rzr0153e	feeder_id;	/* feeder id			*/
	unsigned char	min_amp;	/* minimun aplitude		*/
	unsigned char	max_amp;	/* maximun aplitude		*/
	unsigned char	multiplier;	/* フィーダ時間倍率		*/
	enum rzr0150e	phase_type;	/* 位相				*/
	enum rzr0151e	opt_sw_ctrl;	/* 光電子SWによる停止モード	*/
	enum rzr0152e	shutter_ctrl;	/* シャッタ連動モード		*/
	unsigned char	shutter_off_delay;/* シャッタ停止遅れ時間	*/
} RZR015T;

#if	defined(STORAGE)
struct rzr015t	rzr015s;
#else
#if	!defined(NOEXTERN)
extern struct rzr015t	rzr015s;
#endif
#endif

#endif
