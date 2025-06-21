/*----------------------------------------------------------------------*
 * File name	: mrt100.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt100.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt100.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt100.c 1.1 1996/07/22 19:25:32
 *Log: mrt100.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<custom\custom.h>			/*	"		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"admtxt.h"			/* current		*/
#define	STORAGE
#include	"pwbcfg.h"			/*	"		*/
#undef	STORAGE

/*----------------------------------------------------------------------*/
/*	mon_chg_adc_board						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	アクセス対象となるADCボードを変更する。				*/
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
mon_chg_adc_board(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[2];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	int		i;

	if(((i= htoi(argv[2])-1) >= DAD_MAX)||(i < 0)) {
		mon_text_snd( ILLEGAL_PRM1, argl );		return(-1);
	}
	pwbid = i;
	return( mon_text_snd( CHG_ADC_SUCES, argl) );
}
