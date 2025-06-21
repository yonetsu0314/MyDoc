/*----------------------------------------------------------------------*
 * File name	: scicls.c		/ sci close								*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scicls.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic2/RCS/scicls.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: scicls.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                         		                         		*/
/*		SCIをクローズする。												*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ										*/
/*																		*/
/* 戻り値																*/
/*		int			: 0													*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>							/* Show xlib		*/
#include	<drive\iofsys.h>						/*	"				*/
#include	"scictl.h"								/* Show current		*/

int
scif_close(
	int		channel
	)
{
	channel = channel;	/* ｺﾝﾊﾟｲﾙﾜｰﾆﾝｸﾞ対策 */
	sci_mode = -1;
	return(0);
}

