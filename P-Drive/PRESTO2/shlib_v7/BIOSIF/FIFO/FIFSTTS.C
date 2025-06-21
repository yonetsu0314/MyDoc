/*----------------------------------------------------------------------*
 * File name	: fifstts.c	/ initialize section			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fifstts.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifstts.c $
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
/*	ＦＩＦＯのステータスを読み込む。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void					*/
/*									*/
/* 戻り値								*/
/*	int:ステータス							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>
#include	<umachine.h>
#include	<dos.h>

unsigned char
_fifo_status(void)
{
	unsigned char	err;
	
	err = (unsigned char)trapa_svc(VECTOR_FPGAFIFO, 0x1);
	return(err);
}
