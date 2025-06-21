/*----------------------------------------------------------------------*
 * File name	: comopn.c	/ arcnet open				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comopn.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: comopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comopn.c 1.1 1995/11/07 17:12:41 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARCNETをオープンする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<dos.h>				/* MS-C std		*/
#include	<rom\handle.h>			/* Show xlib		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
/*#include	<custom\custom.h>*/		/* Show xlib		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	"comctl.h"			/* Show current		*/

int
arcnet_open(
	int		channel,
	int		mode
	)
{
	if(channel >= COM_DEV_MAX)				return(-1);
	_disable();
	com_mode[channel] = mode;
	iodrv_read[__ARCNET_-1] = arcnet_rx;
	iodrv_write[__ARCNET_-1] = arcnet_tx;
	iodrv_close[__ARCNET_-1] = arcnet_close;
	_enable();
	return(0);
}
