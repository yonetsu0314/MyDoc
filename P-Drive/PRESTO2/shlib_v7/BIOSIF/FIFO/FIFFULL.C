/*----------------------------------------------------------------------*
 * File name	: fiffull.c	/ initialize register			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fiffull.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fiffull.c $
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
/*	ＦＩＦＯのFULLフラグを読み込む。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void					*/
/*									*/
/* 戻り値								*/
/*	int	: FULLフラグデータ							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

int
_fifo_full(void)
{
	int		err;
	
	err = (int)trapa_svc(VECTOR_FPGAFIFO, 0x5);
	return(err);
}
