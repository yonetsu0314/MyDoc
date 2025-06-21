/*----------------------------------------------------------------------*
 * File name	: mrt008.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt008.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt008.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt008.c 1.1 1970/01/01
 *Log: mrt008.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"admtxt.h"			/* Current		*/
#include	"pwbcfg.h"			/*	"		*/

static unsigned short	stat = (unsigned short)0;

/*----------------------------------------------------------------------*/
/*	mon_dad_stp_sampl						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃﾞｰﾀのｻﾝﾌﾟﾘﾝｸﾞを停止する。					*/
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
mon_dad_stp_sampl(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/

	if(stat == 0)						return(0);
	if(stat&(DAD_ORG|DAD_1ST|DAD_2ND|DAD_3RD|DAD_4TH|DAD_5TH)) {
		if(dad_stp_mon( pwbid))	goto fail;
	}
	if(stat&((DAD_AFV0_3RD|DAD_AFV1_3RD)<<5)) {
		if(dad_stp_dsp_mon( pwbid)) {
fail:			mon_text_snd( STOP_FAIL, argl);		return(-1);
		}
	}
	stat = (unsigned short)0;
	return(mon_text_snd( STOP_SUCES, argl));
}

/*----------------------------------------------------------------------*/
/*	mon_save_sampl_stat						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾓﾆﾀｰ情報をｾｰﾌﾞする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	sts		: ﾓﾆﾀｰするｽﾃｰｼﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*----------------------------------------------------------------------*/

int
mon_save_sampl_stat(
	unsigned short	sts
	)
{
	stat = sts;
	return(0);
}
