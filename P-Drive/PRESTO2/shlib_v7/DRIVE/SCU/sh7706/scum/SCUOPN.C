/*----------------------------------------------------------------------*
 * File name	: scuopn.c	/ scu open				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scum/RCS/scuopn.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_scu/rcs/scuopn.c 1.1 1993/10/20 14:38:00 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	SCUをオープンする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0						*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<intrpt.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"scuctl.h"			/* Show current		*/

int
scu_open(
	int		ch,
	int		mode
	)
{
	_disable();
	scu_mode = mode;
	sctx[ch].putp = sctx[ch].getp = sctx[ch].lcnt = (unsigned short)0;
	scrx[ch].putp = scrx[ch].getp = scrx[ch].lcnt = (unsigned short)0;
	iodrv_read[__SCU_-1] = scu_rx;
	iodrv_write[__SCU_-1] = scu_tx;
	iodrv_close[__SCU_-1] = scu_close;
	_enable();
	return(0);
}
