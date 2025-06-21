/*----------------------------------------------------------------------*
 * File name	: dad3int.c	/ dadc#3 RX interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dad3int.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dad3int.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCﾎﾞｰﾄﾞ(DP-SRAM)の受信割り込みﾊﾝﾄﾞﾗ。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\dadc.h>			/* Show biosif		*/
#include	<drive\pdadcdrv.h>		/*	"		*/
#include	"dadctl.h"			/* show current		*/
#include	"dadctxt.h"			/*	"		*/

#define	PWB4	3

void dadc3_int_hdr(void)
{
	WUP_TSK(dadi_tsk[PWB4].acadr);
	return;
}
