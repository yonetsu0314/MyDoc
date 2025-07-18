/*----------------------------------------------------------------------*
 * File name	: tmrint.c	/ timer interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/timer/RCS/tmrint.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tmrint.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	割り込みハンドラ。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	"tmrctl.h"			/* Show current		*/


//#pragma interrupt(tmr_int_hdr)

void tmr_int_hdr(void)
{
	WUP_TSK(tmr_tsk.acadr);
	return;
}
