/*======================================================================
 * File name    : rzr013.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr013.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr013.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr013.hv  2.1  93/11/02 15:41:06  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr013.hv
 * Revision 2.1  93/11/02 15:41:06  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR013_H)
#define	RZR013_H

enum rzr0130e {				/* ホッパの種類			*/
	HOPPER_PH,			/* ＰＨ				*/
	HOPPER_WH,			/* ＷＨ				*/
	HOPPER_BH,			/* ＢＨ				*/
	HOPPER_DTH,			/* ＤＴＨ			*/
	HOPPER_TH,			/* ＴＨ				*/
	HOPPER_RS,			/* ＲＳ				*/
};

enum rzr0131e {				/* アクチュエータ		*/
	ACT_SM,				/* ステッピングモータ		*/
	ACT_CB,				/* クラッチ＆ブレーキ		*/
};

enum rzr0132e {				/* 励磁タイプ			*/
	PHASE_1 = 1,			/* １相				*/
	PHASE_2,			/* ２相				*/
	PHASE_12,			/* １－２相			*/
	PHASE_W12,			/* Ｗ－１－２相			*/
};

enum rzr0133e {				/* 停止条件			*/
	STOP_NON,			/* なし				*/
	STOP_LGHT,			/* 入光				*/
	STOP_DARK,			/* 遮光				*/
	STOP_EDGE,			/* エッジ			*/
	STOP_PLS,			/* パルス			*/
	STOP_PLS_LGHT,			/* パルス＋入光			*/
	STOP_PLS_DARK,			/* パルス＋遮光			*/
	STOP_PLS_EDGE,			/* パルス＋エッジ		*/
};

typedef struct rzr0130t {
	unsigned char	pulse_mltp;	/* 基本パルス倍数		*/
	unsigned char	repeat;		/* 繰り返し数			*/
} RZR0130T;

/*------------------------------------------------------*/
/*	ホッパ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr013t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	hop_assign_ptn;	/* Hopper assign pattern
					   [nof_head/4]			*/
	unsigned short	open_time;	/* open time			*/
	enum rzr0130e	hopper_id;	/* hopper id			*/
	unsigned char	hopper_no;	/* hopper number		*/
	enum rzr0131e	actuator_type;	/* アクチュエータタイプ		*/
	enum rzr0132e	phase_type;	/* 励磁モード	3:1-2相		*/
	enum rzr0133e	stop_mode;	/* 停止条件			*/
	unsigned short	speed;		/* 回転数 [単位パルス時間0.4us] */
	unsigned short	ignore_pulse;	/* スリット不感パルス数	 	*/
       	unsigned char	cam_slit;	/* スリット停止パルス数 	*/
  	unsigned char	br_cl_delay;	/* BR-CL動作遅れパルス数	*/
	unsigned char	cl_br_delay;	/* CL-BR動作遅れパルス数	*/
	unsigned char	brake_time;	/* ブレーキ時間	      [10msec]	*/
	unsigned char	error_pulse;	/* エラー検出パルス数		*/
	unsigned char	nof_pattern;	/* パターン数			*/
	struct rzr0130t	ary[128];	/* 駆動パターン [nof_pattern]	*/
} RZR013T;

#if	defined(STORAGE)
struct rzr013t	rzr013s;
#else
#if	!defined(NOEXTERN)
extern struct rzr013t	rzr013s;
#endif
#endif

#endif
