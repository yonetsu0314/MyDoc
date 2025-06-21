/*----------------------------------------------------------------------*
 * File name	: mrt007.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt007.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt007.c 1.1 1970/01/01
 *Log: mrt007.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	"drive\pdadcdrv.h"		/* Show drive		*/
#include	"drive\dadcsim.h"		/* 	"		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

/*----------------------------------------------------------------------*/
/*	mon_dad_reset							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCにｿﾌﾄｳｪｱｰ･ﾘｾｯﾄ･ｺﾏﾝﾄﾞを発行する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc		: no. of argumunt				*/
/*	*argv[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
mon_dad_reset(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/

	if(dad_reset(pwbid) == -1) {
		mon_text_snd( RESET_FAIL, argl);		return(-1);
	}
	return( mon_text_snd( RESET_SUCES, argl) );
}
