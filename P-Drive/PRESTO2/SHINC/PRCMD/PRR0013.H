/*======================================================================
 * File name	: prr0013.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0013.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0013.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr013.hv  2.1  93/11/02 15:41:06  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr013.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0013_H)
#define	PRR0013_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

enum prr00131e {			/* アクチュエータ		*/
	ACT_SM,				/* ステッピングモータ		*/
	ACT_CB,				/* クラッチ＆ブレーキ		*/
};

enum prr00132e {			/* 励磁タイプ			*/
	PHASE_1 = 1,			/* １相				*/
	PHASE_2,			/* ２相				*/
	PHASE_12,			/* １－２相			*/
	PHASE_W12,			/* Ｗ－１－２相			*/
};

enum prr00133e {			/* 停止条件			*/
	STOP_NON,			/* なし				*/
	STOP_LGHT,			/* 入光				*/
	STOP_DARK,			/* 遮光				*/
	STOP_EDGE,			/* エッジ			*/
	STOP_PLS,			/* パルス			*/
	STOP_PLS_LGHT,			/* パルス＋入光			*/
	STOP_PLS_DARK,			/* パルス＋遮光			*/
	STOP_PLS_EDGE,			/* パルス＋エッジ		*/
};

typedef struct prr00130t {
	unsigned char	pulse_mltp;	/* 基本パルス倍数		*/
	unsigned char	repeat;		/* 繰り返し数			*/
} PRR00130T;

/*------------------------------------------------------*/
/*	ホッパ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0013t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	hop_assign_ptn;	/* Hopper assign pattern	*/
	unsigned short	open_time;	/* open time			*/
	enum prprm004e	hopper_id;	/* hopper id			*/
	unsigned char	hopper_no;	/* hopper number		*/
	enum prr00131e	actuator_type;	/* アクチュエータタイプ		*/
	enum prr00132e	phase_type;	/* 励磁モード	3:1-2相		*/
	enum prr00133e	stop_mode;	/* 停止条件			*/
	unsigned short	speed;		/* 回転数 [単位パルス時間0.4us] */
	unsigned short	ignore_pulse;	/* スリット不感パルス数		*/
	unsigned char	cam_slit;	/* スリット停止パルス数		*/
	unsigned char	br_cl_delay;	/* BR-CL動作遅れパルス数	*/
	unsigned char	cl_br_delay;	/* CL-BR動作遅れパルス数	*/
	unsigned char	brake_time;	/* ブレーキ時間	[10msec]	*/
	unsigned char	error_pulse;	/* エラー検出パルス数		*/
	unsigned char	nof_pattern;	/* パターン数			*/
	struct prr00130t	ary[128];/* 駆動パターン [nof_pattern]	*/
} PRR0013T;

#if	defined(STORAGE)
struct prr0013t	prr0013s;
#else
#if	!defined(NOEXTERN)
extern struct prr0013t	prr0013s;
#endif
#endif

#endif
