/*----------------------------------------------------------------------*
 * File name	: comcls.c	/ arcnet close				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comcls.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: comcls.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comcls.c 1.1 1995/11/07 17:13:08 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARCNETをクローズする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"comctl.h"			/* Show current		*/

int
arcnet_close(
	int		channel
	)
{
	if(channel >= COM_DEV_MAX)				return(-1);
	com_mode[channel] = -1;
	return(0);
}
