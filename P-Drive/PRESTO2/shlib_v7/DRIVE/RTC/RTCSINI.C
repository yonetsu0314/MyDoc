/*----------------------------------------------------------------------*
 * File name	: rtcsini.c	/ rtc system initialize			*
 * Original	: p:/lib/drive/rtc/rcs/rtcsini.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcsini.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rtcsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * リアルタイム・クロック
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY			*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｾﾏﾌｫ及びBIOSｺﾝﾃｷｽﾄの生成を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し							*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			: -1 ･･･異常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\rtc.h>		/*	"		*/
#include	<drive\rtcdrv.h>		/*	"		*/
#include	<drive\rxdefd.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"rtcctl.h"			/*	"		*/
#undef		STORAGE

int
rtc_sys_init(void)
{
	int						err;

	if(err = CRE_SEM( FIFO, &rtc_sem.acadr,rtc_sem_id, 1))
		return(-1);
	if(err = WAI_SEM( ENDLESS, rtc_sem.acadr,1, VOID_TIME))
		return(-1);
	if(_rtc_initialize() == -1)
		return(-1);
	return(0);
}
