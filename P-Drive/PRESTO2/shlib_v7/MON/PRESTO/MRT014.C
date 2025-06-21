/*----------------------------------------------------------------------*
 * File name	: mrt014.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt014.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt014.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt014.c 1.1 1970/01/01
 *Log: mrt014.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<limits.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	"afv001.h"			/* Show apl		*/
#include	"bkf100.h"			/* Show apl		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

#define	PTN_MAX	(sizeof(xs)*CHAR_BIT)

/*----------------------------------------------------------------------*/
/*	mon_dsp_sta_afv							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV調整を行う。AFVの号機指定がない場合は、..\aplの		*/
/*	afv_adj_tsk()を起床する。					*/
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
mon_dsp_sta_afv(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[2];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	DAD_ARY_PTN	xs ={0};		/* 指定ﾍｯﾄﾞ･ﾊﾟﾀｰﾝ	*/
	DAD_ARY_PTN	exe_bp[2];		/* 実行結果  "		*/
	int		len, n, j;
	char		*cp, *pp;

	if(argc != 2) {
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}
	if((len = strlen(argv[2])) > 62) {
		mon_text_snd( ILLEGAL_NO_PRM1, argl);		return(-1);
	}
	n = 0;
	pp = cp = argv[2];
	while((cp = strchr( pp, PUNCTUATOR)) != NULL) {
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) > 3)  goto err_retp1n;
		if(((j = atoi(pp)) > PTN_MAX)||(j == 0))  goto err_retp1;
		if(j<=DAD_WEI_MAX){
			xs.wei_bp |= 0x1L << (j-1);
		} else if(j<=(DAD_WEI_MAX+DAD_DF_MAX)){
			xs.df_bp |= 0x1L << (j-DAD_WEI_MAX-1);
		} else {
			xs.afv_bp |= 0x1L << (j-DAD_WEI_MAX-DAD_DF_MAX-1);
		}
		pp = cp;
		n++;
	}
	if(n >= PTN_MAX)  goto err_retp1;
	if((len = strlen(pp)) > 3) {
err_retp1n:	mon_text_snd( ILLEGAL_NO_PRM1, argl);		return(-1);
	}
	if(((j = atoi(pp)) > PTN_MAX)||(j == 0)) {
err_retp1:	mon_text_snd( ILLEGAL_PRM1, argl);		return(-1);
	}
	if(j<=DAD_WEI_MAX){
		xs.wei_bp |= 0x1L << (j-1);
	} else if(j<=(DAD_WEI_MAX+DAD_DF_MAX)){
		xs.df_bp |= 0x1L << (j-DAD_WEI_MAX-1);
	} else {
		xs.afv_bp |= 0x1L << (j-DAD_WEI_MAX-DAD_DF_MAX-1);
	}
	exe_bp[0] = xs;
	exe_bp[1].wei_bp = 0;
	exe_bp[1].df_bp = 0;
	exe_bp[1].afv_bp = 0;
	argl[0] = &exe_bp[0];
	argl[1] = &afvc[pwbid]->rev;

	return( sta_afv_tsk( xs.wei_bp ));
}
