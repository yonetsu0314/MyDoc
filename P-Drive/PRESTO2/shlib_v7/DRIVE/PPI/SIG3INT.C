/*----------------------------------------------------------------------*
 * File name	: sig3int.c	/ signal #3 interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ppi/RCS/sig3int.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sig3int.c $
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
/*	SIGNAL#3割り込みハンドラ。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	"pioctl.h"			/* Show current		*/

#define	TASK_NO		3

//#pragma interrupt(sig3_int_hdr)

void sig3_int_hdr(void)
{
	WUP_TSK(sig_int_tsk[TASK_NO]->acadr);
}
