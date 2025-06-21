/*----------------------------------------------------------------------*
 * File name	: sramcls.c	/ sram close				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramcls.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramcls.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramcls.c 1.2 1995/10/09 15:49:16 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	SRAMをクローズする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	id		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/*	"		*/
#include	<rom\handle.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	"sramctl.h"			/* Show current		*/

int
sram_close(
	int id
	)
{
	int		err = 0;

	if(id >= SRAM_DEV_MAX)				return(-1);
	sram_mode[id] = -1;
	ADV_SIG_SEM(err, sram_nest[id], sram_sem[id].acadr);
	if(err)						return(err|INT_MIN);
	return(0);
}
