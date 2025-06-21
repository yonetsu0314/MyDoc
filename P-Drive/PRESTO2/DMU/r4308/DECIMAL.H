/*======================================================================
 * File name    : decimal.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: アスキー１０進文字の変換
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/decimal.h 1.1 1998/08/26 18:29:47 yonetsu Exp $
 *$Log: decimal.h $
 * リビジョン 1.1  1998/08/26  18:29:47  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-1998, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#ifndef _DECIMAL_H_
#define _DECIMAL_H_

int adec2i(int size,const char *adec_p);
long adec2l(int size,const char *adec_p);
char *i2adec(int val,int size,char *adec_p);
char *l2adec(long val,int size,char *adec_p);

#endif
