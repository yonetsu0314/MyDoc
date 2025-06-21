/*----------------------------------------------------------------------*
 * File name	: flshwr.c	/ flash rom word write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/flash/rcs/flshwr.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: flshwr.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ５種新関数追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	FLASH ROMに16BIT 1WORD書き込みを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	adr	: アドレス				*/
/*	data	: データ			*/
/*									*/
/* 戻り値								*/
/*	int	: ０：正常　１：ＮＧ　２：タイムアウト					*/
/*----------------------------------------------------------------------*/

#include	<kernel.h>
#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<dos.h>

W
_flash_write(H *adr,H data)
{
	W	err;
	
	err = (int)trapa_svc(VECTOR_FROM, 0x2, adr, data);
	return(err);
}
