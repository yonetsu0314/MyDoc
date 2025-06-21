/*----------------------------------------------------------------------*
 * File name	: mst005.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mst005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mst005.c 1.1 1970/01/01
 *Log: mst005.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	"admtxt.h"			/* current		*/

/*----------------------------------------------------------------------*/
/*	mon_cboot_text							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCﾁｪﾝｼﾞ･ﾌﾞｰﾄ終了のｽﾃｰﾀｽ･ﾃｷｽﾄを作成する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf	: pointer to string buffer				*/
/*	*argl[]	: pointer to argument[]					*/
/*			arg[0]	･･･ unsigned short *			*/
/*				    3RD filter time constant		*/
/*			arg[1]	･･･ unsigned short *			*/
/*				    4TH filter time constant		*/
/*									*/
/* 戻り値								*/
/*	char *	: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ					*/
/*----------------------------------------------------------------------*/

char *
mon_cboot_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{

	strcat( buf, "\n\r\tdata inhibit:");
	sprintf( buf+ strlen(buf), "  3RD = %4d ms,", *(short *)argl[0] *10);
	sprintf( buf+ strlen(buf), "  4TH = %4d ms", *(short *)argl[1] *10);
	sprintf( buf+ strlen(buf), "  5TH = %4d ms", *(short *)argl[2] *10);
	return( buf+ strlen(buf));
}
