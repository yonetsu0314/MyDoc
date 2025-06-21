/*----------------------------------------------------------------------*
 * File name	: dpmcls.c	/ dpmem close				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmcls.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmcls.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmcls.c 1.1 1993/10/20 20:03:52 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	DP-SRAMをクローズする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"dpmctl.h"			/* Show current		*/

int
dpm_close(
	int		channel
	)
{
	if(channel >= DPM_DEV_MAX)				  return(-1);
	dpm_mode[channel] = -1;
	return(0);
}
