/*----------------------------------------------------------------------*
 * File name	: fifinp.c	/ initialize register			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fifinp.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifinp.c $
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
/*	ＦＩＦＯのデータ入力を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void					*/
/*									*/
/* 戻り値								*/
/*	unsigned char	: ＦＩＦＯ読出しデータ							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

unsigned char
_fifo_input(void)
{
	unsigned char	err;
	
	err = (unsigned char)trapa_svc(VECTOR_FPGAFIFO, 0x0);
	return(err);
}
