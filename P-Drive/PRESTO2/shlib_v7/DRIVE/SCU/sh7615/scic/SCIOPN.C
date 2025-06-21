/*----------------------------------------------------------------------*
 * File name	: sciopn.c		/ sci open								*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/sciopn.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic/RCS/sciopn.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: sciopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		SCIをオープンする。												*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ										*/
/*		mode		: ｱｸｾｽ･ﾓｰﾄﾞ											*/
/*																		*/
/* 戻り値																*/
/*		int			: 0													*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>						/* Show xlib			*/
#include	<drive\iofsys.h>					/*	"					*/
#include	"scictl.h"							/* Show current			*/
#include	"scictxt.h"							/* Show current			*/

int
scif_open(
	int		ch,
	int		mode
	)
{
	sctx[ch].putp = sctx[ch].getp = sctx[ch].lcnt = (unsigned short)0;
	scrx[ch].putp = scrx[ch].getp = scrx[ch].lcnt = (unsigned short)0;

	iodrv_read[__SCU_-1] = (void *)scif_rx;
	iodrv_write[__SCU_-1] = (void *)scif_tx;
	iodrv_close[__SCU_-1] = (void *)scif_close;

	return(0);
}
