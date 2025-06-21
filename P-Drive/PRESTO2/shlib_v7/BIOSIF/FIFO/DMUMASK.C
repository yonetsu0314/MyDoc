/*----------------------------------------------------------------------*
 * File name	: dmumask.c	/ initialize vector			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/dmumask.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: dmumask.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * DMUにint_mask,pint_maskを追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＤＭＵの外部割込みのマスクの設定を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	int		mask					*/
/*									*/
/* 戻り値								*/
/*	なし							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

void
_dmu_int_mask(int mask)
{
	trapa_svc(VECTOR_FPGAFIFO, 0x6, mask);
}
