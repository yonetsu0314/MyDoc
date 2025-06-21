/*======================================================================
 * File name    : bkf005.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 * 	ＡＤＣボードセットアップ変数定義
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/rz_wcu2/rcs/bkf005.h 1.1.6510.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: bkf005.h $
 * リビジョン 1.1.6510.1  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:09
 * ＳＨ３に対するワーニング対策
 * 
 * リビジョン 1.1  1996/06/18  18:41:13  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/rz_wcu/rcs/bkf005.h 2.1 1993/10/20 11:20:02 ryoji
 *Log: bkf005.h
 * リビジョン 2.1  1993/10/20  11:20:02  ryoji
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		BKF005_H
#define		BKF005_H

#ifndef	ADCS_H
#include	"adcs.h"			/* Show Current		*/
#endif

/*--------------------------------------------------------------*/
/*	ADパラメータの構造体定義				*/
/*								*/
/*--------------------------------------------------------------*/

enum adrng {
	RNG_400,			/* 400 gram mode		*/
	RNG_800,			/* 800 gram mode		*/
	RNG_MASK = 0x0f
};

enum adavg {
	JUDGE_CTMIN = 1,		/* MIN安定判断比較回数		*/
	JUDGE_CTMAX = 0x0f,		/* MAX安定判断比較回数		*/
	AVG_CNT  = (0x2 << 4)		/* AD平均化回数(上位4bit)	*/
};

typedef struct adpr {
	int		stable_weight;	/* 安定重量幅			*/
	int		zero_limit;	/* 自動ゼロ点安定カウント  	*/
	enum adavg	average;	/* AD平均化回数			*/
	enum adrng	range;		/* 秤レンジ			*/
	unsigned char	max_ch;		/* 最大 ADチャネル数		*/
	unsigned char	cell_type;	/* 使用セルタイプ		*/
	int		interval;	/* スケール入力間隔		*/
	int		smoothing;	/* 平均化処理判別カウント	*/
	int		span_limit;	/* スパンチェック安定カウント	*/
	int		int_interval;	/* AD割り込み間隔		*/
     	int		int_enable;	/* AD割り込み許可フラグ		*/
} ADPR;


#if defined(STORAGE)
struct	adpr	adpr_ini = {
	10, 				/* 安定重量幅			*/
	80,                             /* 自動ゼロ点安定カウント  	*/
	AVG_CNT+JUDGE_CTMIN,            /* AD平均化回数			*/
	RNG_400,                        /* 秤レンジ			*/
	(unsigned char)AD_CH_MAX,       /* 最大 ADチャネル数		*/
	(unsigned char)3,               /* 使用セルタイプ		*/
	20,                             /* スケール入力間隔		*/
	3,                              /* 平均化処理判別カウント	*/
	80,                             /* スパンチェック安定カウント	*/
	4,                              /* AD割り込み間隔		*/
	0x60                            /* AD割り込み許可フラグ		*/
};
#else
extern struct adpr	adpr_ini;
extern struct adpr	*const adprm[];	/* ADパラメータへのポインタ	*/
#endif

#endif
