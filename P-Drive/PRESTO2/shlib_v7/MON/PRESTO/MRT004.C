/*----------------------------------------------------------------------*
 * File name	: mrt004.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt004.c 1.1 1970/01/01
 *Log: mrt004.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"bkf004.h"			/* Show apl		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

#define MAX_NO	(DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX)

/*----------------------------------------------------------------------*/
/*	mon_dad_mzero							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	手動零点補正を行う。						*/
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
mon_dad_mzero(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	DAD_ARY_PTN	xs ={0};		/* 指定ﾍｯﾄﾞ･ﾊﾟﾀｰﾝ	*/
	DAD_ARY_PTN	exe_bp[2];		/* 実行結果  "		*/
	short		err_sts[sizeof(xs)*CHAR_BIT];
	int		len, err, i, n, j;
	char		*cp, *pp;

	if(argc != 2) {
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}
	if((len = strlen(argv[2])) >= 60) {
		mon_text_snd( ILLEGAL_NO_PRM1, argl);		return(-1);
	}
	n = 0;
	pp = cp = argv[2];
	while((cp = strchr( pp, PUNCTUATOR)) != NULL) {
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) > 3)  goto err_retp1n;
		if(((j = atoi(pp)) > MAX_NO)||(j == 0))  goto err_retp1;
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
	if(n >= MAX_NO)  goto err_retp1;
	if((len = strlen(pp)) > 3) {
err_retp1n:	mon_text_snd( ILLEGAL_NO_PRM1, argl);		return(-1);
	}
	if(((j = atoi(pp)) > MAX_NO)||(j == 0)) {
err_retp1:	mon_text_snd( ILLEGAL_PRM1, argl);		return(-1);
	}
	if(j<=DAD_WEI_MAX){
		xs.wei_bp |= 0x1L << (j-1);
	} else if(j<=(DAD_WEI_MAX+DAD_DF_MAX)){
		xs.df_bp |= 0x1L << (j-DAD_WEI_MAX-1);
	} else {
		xs.afv_bp |= 0x1L << (j-DAD_WEI_MAX-DAD_DF_MAX-1);
	}
	exe_bp[0] = xs ;
	argl[0] = &exe_bp[0];
	argl[1] = &err_sts[0];
	argl[2] = &adzero[pwbid]->da[0];
	memcpy((void *)&err_sts[0], (void *)&adzero[pwbid]->ad[0], sizeof(err_sts));
	if((err = dad_mzero( pwbid, xs, &exe_bp[1], &err_sts[0],
			     &adzero[pwbid]->ad[0],
			     &adzero[pwbid]->da[0])) == -1) {
		mon_text_snd( ZERO_FAIL, argl);			return(-1);
	}
	for(xs.wei_bp &= ~exe_bp[1].wei_bp, i=0; xs.wei_bp ; xs.wei_bp>>=1, i++) {
		if(xs.wei_bp & 0x1){
			 err_sts[i] = adzero[pwbid]->ad[i];
		}
	}
	for(xs.df_bp &= ~exe_bp[1].df_bp, i=0; xs.df_bp ; xs.df_bp>>=1, i++) {
		if(xs.df_bp & 0x1){
			err_sts[i+DAD_WEI_MAX] = adzero[pwbid]->ad[i+DAD_WEI_MAX];
		}
	}
	for(xs.afv_bp &= ~exe_bp[1].afv_bp, i=0; xs.afv_bp ; xs.afv_bp>>=1, i++) {
		if(xs.afv_bp & 0x1){
			err_sts[i+DAD_WEI_MAX+DAD_DF_MAX] = adzero[pwbid]->ad[i+DAD_WEI_MAX+DAD_DF_MAX];
		}
	}
	return( mon_text_snd( ZERO_SUCES, argl) );
}
