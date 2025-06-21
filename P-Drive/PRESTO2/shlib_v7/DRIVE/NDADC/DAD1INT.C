/*----------------------------------------------------------------------*
 * File name	: dad1int.c	/ dadc#1 RX interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dad1int.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dad1int.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dad1int.c 1.1 1996/07/22 13:36:10 chimura
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
#include	<drive\ndadcdrv.h>		/*	"		*/
#include	"dadctl.h"			/* show current		*/
#include	"dadctxt.h"			/*	"		*/

#define	FI_COM	0x20
#define	PWB2	1

//#pragma interrupt(dadc1_int_hdr)
void dadc1_int_hdr(void)
{
	WUP_TSK(dadi_tsk[PWB2].acadr);
	return;
}
