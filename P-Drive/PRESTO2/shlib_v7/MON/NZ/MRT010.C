/*----------------------------------------------------------------------*
 * File name	: mrt010.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt010.c 1.1 1996/07/22 19:25:04
 *Log: mrt010.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"adc101.h"			/* Show apl		*/
#include	"bkf004.h"			/*	"		*/
#include	"bkf100.h"			/*	"		*/
#include	"admtxt.h"			/* Current		*/
#include	"pwbcfg.h"			/*	"		*/

/*----------------------------------------------------------------------*/
/*	mon_dsp_iboot							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｲﾆｼｬﾙのﾊﾟﾗﾒｰﾀ･ﾌﾞｰﾄ処理を行う。					*/
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
mon_dsp_iboot(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	char		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	struct dadpk10	boot_pk;		/* parameter packet	*/
	int		err, len, i;
	long		n;
	char		*p;

	if(argc != 4) {
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}
	if((i = atol(argv[2])) <= 0) {
		mon_text_snd( ILLEGAL_PRM1, (void **)argl);	return(-1);
	}
	boot_pk.wei_max = (short)i;
	if((i = atol(argv[3])) <= 0) {
		mon_text_snd( ILLEGAL_PRM2, (void **)argl);	return(-1);
	}
	boot_pk.afv_max = (short)i;
	if((i = atol(argv[4])) <= 0) {
		mon_text_snd( ILLEGAL_PRM3, (void **)argl);	return(-1);
	}
	boot_pk.df_max = (short)i;
	for(n= 0, i= 0; i< DAD_WEI_MAX; n += afvc[pwbid]->rev[i], ++i);
	boot_pk.afv_typ = n/ DAD_WEI_MAX;
	boot_pk.wei_spc = &adspan[pwbid]->da[0];
	boot_pk.df_spc = &adspan[pwbid]->da[DAD_WEI_MAX];
	boot_pk.afv_spc = &adspan[pwbid]->da[DAD_WEI_MAX+DAD_DF_MAX];
	boot_pk.afv_fprm = (void *)&afvc[pwbid]->rev;
	boot_pk.x_ary = (void *)&adb[pwbid].coord_x[0];
	boot_pk.y_ary = (void *)&adb[pwbid].coord_y[0];
	boot_pk.wei_zad = &adzero[pwbid]->ad[0];
	boot_pk.df_zad = &adzero[pwbid]->ad[DAD_WEI_MAX];
	boot_pk.afv_zad = &adzero[pwbid]->ad[DAD_WEI_MAX+DAD_DF_MAX];

	err = dad_iboot0( pwbid, &boot_pk );
	if(err == -1) {
		mon_text_snd( DSP_IBOOT_FAIL, (void **)argl);	return(-1);
	}
	return( mon_text_snd( DSP_IBOOT_SUCES, (void **)argl) );
}
