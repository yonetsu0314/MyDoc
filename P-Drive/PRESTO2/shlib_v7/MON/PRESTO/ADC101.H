/*----------------------------------------------------------------------*/
/* File name	: adc101.h	/ afv coefficient struct		*/
/*----------------------------------------------------------------------*
 *$Header: p:/wcu/main/dz_wcu2/rcs/adc101.h 1.1.6308.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: adc101.h $
 * Revision 1.1.6308.2  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:05
 * コメント記号のネスティングを修正する
 * 
 * リビジョン 1.1.6308.1  1970/01/01  00:00:00  chimura
 * 2003/03/19 14:30
 * dzr211sの二重定義を回避する。
 * 
 * リビジョン 1.1  1996/06/18  18:40:48  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/dz_wcu/rcs/adc101.h 3.1 1994/03/18 16:27:38 kabumoto
 *Log: adc101.h
 * リビジョン 3.1  1994/03/18  16:27:38  kabumoto
 *----------------------------------------------------------------------*
 *
 *
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#ifndef		ADC101_H
#define		ADC101_H

#ifdef	STORAGE
#undef	STORAGE
#include	<dzcmd\dzr211.h>		/* Show dzcmd		*/
#define	STORAGE
#else
#include	<dzcmd\dzr211.h>		/* Show dzcmd		*/
#endif

#include	<drive\dadcdrv.h>		/* Show drive		*/

/*----------------------------------------------*/
/*	ﾍｯﾄﾞの重心のXY座標ﾃﾞｰﾀ			*/
/*						*/
/*----------------------------------------------*/

#ifdef	STORAGE
struct dzr211t	adb[DAD_MAX];			/* X,Y座標, AFV係数	*/

#else
extern struct dzr211t	adb[DAD_MAX];

#endif

#endif
