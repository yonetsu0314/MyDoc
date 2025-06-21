/*----------------------------------------------------------------------*
 * File name	: cdchk.c	/ CF card detect		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/cf/rcs/cdchk.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cdchk.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 05.03.18 CD_CHKをBIOSに移動
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	CFが搭載されているか検出する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	psock		: socket No.					*/
/*									*/
/* 戻り値								*/
/*	int	:  	1･･･ success		*/
/*			  	0	･･･ fail		*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

int
_cd_chk(
	int		psock			/* board number		*/
	)
{
	int err;
	
	err = (int)trapa_svc(VECTOR_CF, 0x1, psock);
	return(err);
}
