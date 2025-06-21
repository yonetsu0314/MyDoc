/*----------------------------------------------------------------------*
 * File name	: mrt011.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt011.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt011.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt011.c 1.1 1970/01/01
 *Log: mrt011.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"adc100.h"			/* Show apl		*/
#include	"adc103.h"			/*	"		*/
#include	"bkf005.h"			/*	"		*/
#include	"admtxt.h"			/* Current		*/
#include	"pwbcfg.h"			/*	"		*/

static const int	wmt[] = {		/* 計量ﾚﾝｼﾞ変換ﾃｰﾌﾞﾙ	*/
	W400G,	W800G,	W1600G,	W1000G,	W2000G,
	W4000G,	W40G,	W80G
};

/*----------------------------------------------------------------------*/
/*	mon_dsp_cboot							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾁｪﾝｼﾞ･ﾊﾟﾗﾒｰﾀ･ﾌﾞｰﾄ処理を行う。					*/
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
mon_dsp_cboot(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	char		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	struct dadpk11	boot_pk;		/* parameter packet	*/
	int		err, i;

	if(argc > 2) {
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}
	if(argc == 2) {
		if((i = atol(argv[2])) < 0) {
			mon_text_snd( ILLEGAL_PRM1, (void **)argl); return(-1);
		}
		adcpub.rcu.afv_mode = i;
	}
	boot_pk.code = (short)(adwmt[adprm[pwbid]->range + adscv[adcpub.rcu.span_wt]]
		       | ((adcpub.rcu.cell_type+1)<<8));
	boot_pk.spn_mas = (short)(adcpub.rcu.span_wt+1);
	boot_pk.afv_sw = (short)adcpub.rcu.afv_mode;
	boot_pk.f3buf_cnt = adcpub.f3buf_cnt;
	boot_pk.f3stb_cnt = adcpub.f3stb_cnt;
	boot_pk.f3avg_cnt = adcpub.f3avg_cnt;
	boot_pk.f4buf_cnt = adcpub.f4buf_cnt;
	boot_pk.f4stb_cnt = adcpub.f4stb_cnt;
	boot_pk.f4avg_cnt = adcpub.f4avg_cnt;
	boot_pk.f5buf_cnt = adcpub.f5buf_cnt;
	boot_pk.f5stb_cnt = adcpub.f5stb_cnt;
	boot_pk.f5avg_cnt = adcpub.f5avg_cnt;
	boot_pk.zr_lmt = (void *)&zero_lmt;

	err = dad_cboot0( pwbid, &boot_pk );
	if(err == -1) {
		mon_text_snd( DSP_CBOOT_FAIL, (void **)argl);	return(-1);
	}
	return( mon_text_snd( DSP_CBOOT_SUCES, (void **)argl) );
}
