/*----------------------------------------------------------------------*/
/* File name	: adc103.h	/ zero ad/da struct			*/
/*----------------------------------------------------------------------*
 *$Header: p:/wcu/main/dz_wcu2/rcs/adc103.h 1.1.6510.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: adc103.h $
 * Revision 1.1.6510.1  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:08
 * ＳＨ３に対するワーニング対策
 * 
 * リビジョン 1.1  1996/06/18  18:40:50  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/dz_wcu/rcs/adc103.h 2.1 1994/03/31 22:14:00 kabumoto
 *Log: adc103.h
 * リビジョン 2.1  1994/03/31  22:14:00  kabumoto
 *----------------------------------------------------------------------*
 *
 *
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

enum {
	ZUPLMT,				/* 変化量の上限値		*/
	ZLOLMT,				/* 変化量の下限値		*/
	ZRELMT,				/* 1回の変化量の許容範囲	*/
	ZLMT_MAX,
};
struct zlmt {				/* 零点許容値の構造体		*/
	short	wei[ZLMT_MAX];		/* 計量器の零点許容範囲		*/
	short	df[ZLMT_MAX];		/* DFの零点許容範囲		*/
	short	afv[ZLMT_MAX];		/* AFVの零点許容範囲		*/
};

#ifdef	STORAGE
struct zlmt	zero_lmt = {
	(short)16000,(short)-16000,(short)166,
	(short)17500,(short)-17500,(short)32767,
	(short)16000,(short)-16000,(short)18
};
#else
extern struct zlmt	zero_lmt;
#endif
