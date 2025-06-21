/*----------------------------------------------------------------------*
 * File name	: wcumask.c	/ initialize vector			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/wcumask.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: wcumask.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04.12.08 WCUMASK追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＷＣＵの外部割込みのマスクの設定を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	unsigned short flag					*/
/*									*/
/* 戻り値								*/
/*	なし							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

void
_wcu_int_mask(int mask)
{
	trapa_svc(VECTOR_FPGAFIFO, 0x6, mask);
}
