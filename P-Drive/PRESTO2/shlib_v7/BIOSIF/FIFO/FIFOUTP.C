/*----------------------------------------------------------------------*
 * File name	: fifoutp.c	/ initialize vector			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fifoutp.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifoutp.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04/12/07 初期レビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＦＩＦＯにデータ出力を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	unsigned char data					*/
/*									*/
/* 戻り値								*/
/*	なし							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

void
_fifo_output(unsigned char data)
{
	trapa_svc(VECTOR_FPGAFIFO, 0x2, data);
}
