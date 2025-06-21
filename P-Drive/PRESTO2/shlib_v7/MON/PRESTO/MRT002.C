/*----------------------------------------------------------------------*
 * File name	: mrt002.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt002.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt002.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt002.c 1.1 1970/01/01
 *Log: mrt002.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<custom\custom.h>		/*	"		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"bkf004.h"			/* Show apl		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

/*----------------------------------------------------------------------*/
/*	mon_dad_iboot							*/
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

union lpuc {
	unsigned long	ptn[2];			/* ADC physical pattern	*/
	unsigned short	us[4];
	unsigned char	uc[8];
};

int
mon_dad_iboot(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	char		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	struct dadpk0	boot_pk;		/* parameter packet	*/
	union lpuc	lbp;
	int		len, i;
	unsigned int	j;
	char		*p;

	if(argc != 4) {
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}
	if((len = strlen(argv[2])) != 8) {
		mon_text_snd( ILLEGAL_NO_PRM1,(void **)argl);	return(-1);
	}
	if((len = strlen(argv[3])) != 8) {
		mon_text_snd( ILLEGAL_NO_PRM2, (void **)argl);	return(-1);
	}
	if((len = strlen(argv[4])) != 8) {
		mon_text_snd( ILLEGAL_NO_PRM3, (void **)argl);	return(-1);
	}
	memset((void *)&boot_pk, 0, sizeof(boot_pk));

	lbp.ptn[0] = 0;
	lbp.ptn[1] = 0;
	for(i =7, p = argv[2]; i >=0 ; i--, p += 2) {
		if((j = htoc(p)) > 0xff) {
			mon_text_snd( ILLEGAL_PRM1, (void **)argl); return(-1);
		}
		lbp.uc[i] = (unsigned char)j;
	}
	boot_pk.ptn.wei_bp = lbp.ptn[0];

	lbp.ptn[0] = 0;
	lbp.ptn[1] = 0;
	for(i =7, p = argv[3]; i >= 0 ; i--, p += 2) {
		if((j = htoc(p)) > 0xff) {
			mon_text_snd( ILLEGAL_PRM2, (void **)argl); return(-1);
		}
		lbp.uc[i] = (unsigned char)j;
	}
	boot_pk.ptn.afv_bp = lbp.us[3];

	lbp.ptn[0] = 0;
	lbp.ptn[1] = 0;
	for(i =7, p = argv[4]; i >= 0 ; i--, p += 2) {
		if((j = htoc(p)) > 0xff) {
			mon_text_snd( ILLEGAL_PRM3, (void **)argl); return(-1);
		}
		lbp.uc[i] = (unsigned char)j;
	}
	boot_pk.ptn.df_bp = lbp.us[4];

	boot_pk.wei_zda = &adzero[pwbid]->da[0];
	boot_pk.df_zda = &adzero[pwbid]->da[DAD_WEI_MAX];
	boot_pk.afv_zda = &adzero[pwbid]->da[DAD_WEI_MAX+DAD_DF_MAX];
	boot_pk.wei_zad = &adzero[pwbid]->ad[0];
	boot_pk.df_zad = &adzero[pwbid]->ad[DAD_WEI_MAX];
	boot_pk.afv_zad = &adzero[pwbid]->ad[DAD_WEI_MAX+DAD_DF_MAX];

	if(dad_iboot1( pwbid, &boot_pk ) == -1) {
		mon_text_snd( IBOOT_FAIL, (void **)argl);	return(-1);
	}
	return( mon_text_snd( IBOOT_SUCES, (void **)argl) );
}
