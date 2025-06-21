/*----------------------------------------------------------------------*/
/* File name	: adc100.h	/ a/d prameter header			*/
/*----------------------------------------------------------------------*
 *$Header: p:/wcu/main/dz_wcu2/rcs/adc100.h 1.1.6308.1.6510.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: adc100.h $
 * リビジョン 1.1.6308.1.6510.1  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:06
 * SH3に対するワーニング対策
 * 
 * リビジョン 1.1.6308.1  1970/01/01  00:00:00  chimura
 * 2003/03/19 14:30
 * dzr209s の二重定義を回避する。
 * 
 * リビジョン 1.1  1996/06/18  18:40:46  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/dz_wcu/rcs/adc100.h 2.2 1994/05/26 10:03:18 kabumoto
 *Log: adc100.h
 * リビジョン 2.2  1994/05/26  10:03:18  kabumoto
 *----------------------------------------------------------------------*
 *
 *
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#ifndef		ADC100_H
#define		ADC100_H

#ifdef	STORAGE
#undef	STORAGE
#include	<drive\dadcdrv.h>		/* Show drive		*/
#include	<dzcmd\dzr009.h>		/* Show dzcmd		*/
#define	STORAGE
#else
#include	<drive\dadcdrv.h>		/* Show drive		*/
#include	<dzcmd\dzr009.h>		/* Show dzcmd		*/
#endif

struct adpub {				/* 計量ﾓｰﾄﾞ･ﾃﾞｰﾀ構造体		*/
	unsigned short	f3rd_tmc;	/* 3RD filter time constant	*/
	unsigned short	f3buf_cnt;	/* 3段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数(MIN =1)	*/
	unsigned short	f3stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f3avg_cnt;	/*   "  平均化処理判定値	*/
	unsigned short	f4th_tmc;	/* 4TH filter time constant	*/
	unsigned short	f4buf_cnt;	/* 4段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数(MIN =1)	*/
	unsigned short	f4stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f4avg_cnt;	/*   "  平均化処理判定値	*/
	unsigned short	f5th_tmc;	/* 4TH filter time constant	*/
	unsigned short	f5buf_cnt;	/* 4段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数(MIN =1)	*/
	unsigned short	f5stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f5avg_cnt;	/*   "  平均化処理判定値	*/
	unsigned short	lose_time;	/* 不確定挙動時間		*/
	struct dzr009t	rcu;		/* ETC				*/
};

#ifdef	STORAGE
struct adpub	adcpub = {
	(unsigned short)21,
	(unsigned short)1,
	(unsigned short)6,
	(unsigned short)80,
	(unsigned short)79,
	(unsigned short)1,
	(unsigned short)6,
	(unsigned short)80,
	(unsigned short)38,
	{ AFV_AUTO, (unsigned char)0, SPN_M200, NLC_12L }
};

const int	adwmt[] = {	/* 計量ﾚﾝｼﾞ変換ﾃｰﾌﾞﾙ	*/
/*      -- 0 --	-- 1 --	-- 2 --	-- 3 --	-- 4 --	-- 5 --	-- 6 --	-- 7 --*/
	W400G,	W800G,	W1600G,	W1000G,	W2000G,	W4000G,	W40G,	W80G,
	W160G
};

const int	adscv[] = {	/* ｽﾊﾟﾝ重量->計量ﾚﾝｼﾞ	*/
	6,	7,	0,	1,	3,	4
};
#else
extern struct adpub	adcpub;
extern const int	adwmt[];
extern const int	adscv[];
#endif
#endif
