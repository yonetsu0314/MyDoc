/*----------------------------------------------------------------------*
 * File name	: cfinit.c	/ initialize cf mem i/o		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/cf/RCS/cfinit.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cfinit.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	CFの物理的情報を得る。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dev_id		: device id					*/
/*									*/
/* 戻り値								*/
/*	const void *	: 物理情報への pointer	･･･ success		*/
/*			  (void  *)0		･･･ fail		*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

const void *
_cf_init(
	int		dev_id			/* board number		*/
	)
{
	void *err;
	
	err = (void *)trapa_svc(VECTOR_CF, 0x0, dev_id);
	return(err);
}
