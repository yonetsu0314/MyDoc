/*----------------------------------------------------------------------*
 * File name	: fpgaver.c	/ version string			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fpgaver.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaver.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * FPGAVERをSH3に追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＦＰＧＡのバージョン情報を取得する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	char *verstr：格納様文字列					*/
/*									*/
/* 戻り値								*/
/*	int	: ０：成功　－１：エラー							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<biosif\fifo.h>	
#include	<umachine.h>
#include	<dos.h>

int
_make_FPGA_verstr(char *verstr)
{
	int		err;
	
	err = (int)trapa_svc(VECTOR_FPGAFIFO, 0x8, verstr);
	return(err);
}
