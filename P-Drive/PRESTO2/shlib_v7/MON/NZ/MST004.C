/*----------------------------------------------------------------------*
 * File name	: mst004.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mst004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mst004.c 1.1 1996/07/22 19:25:20
 *Log: mst004.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

static const char	*sts_msg[] = {
	"illegal sequnce",		/* DAD_MON_HALT			*/
	"terminate",			/* DAD_MON_START		*/
	"buffer full",			/* DAD_MON_SUCES		*/
	"count up",			/* DAD_END_MON			*/
	"recieve NAK",			/* DAD_ERR_CODE0		*/
	"verify error1",		/* DAD_ERR_CODE1		*/
	"verify error2",		/* DAD_ERR_CODE2		*/
	"ACK write error"		/* DAD_ERR_CODE3		*/
};

/*----------------------------------------------------------------------*/
/*	mon_ter_sampl_text						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｻﾝﾌﾟﾘﾝｸﾞ終了のｽﾃｰﾀｽ･ﾃｷｽﾄを作成する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf		: pointer to string buffer			*/
/*	*argl[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	char *		: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ				*/
/*----------------------------------------------------------------------*/

char *
mon_ter_sampl_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
/*	enum h_sts	sts;*/			/* monitor status	*/

	return( buf );
/*	sts = *(int *)argl[0] & (~DAD_MON_TERM);
	strcat( buf, sts_msg[sts]);
	return( buf + strlen(buf));	*/
}
