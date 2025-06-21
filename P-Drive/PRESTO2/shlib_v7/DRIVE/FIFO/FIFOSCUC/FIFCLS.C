/*----------------------------------------------------------------------*
 * File name	: fifcls.c	/ fifo close				*
 * Original	: p:/presto/shlib/drive/scu/RCS/scucls.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoscuc/rcs/fifcls.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifcls.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	FIFOをクローズする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0						*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"fifctl.h"			/* Show current		*/

int
fifo_close(
	int		channel
	)
{
	fif_mode = -1;
	return(0);
}
