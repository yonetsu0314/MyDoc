/*----------------------------------------------------------------------*
 * File name	: mrt005.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt005.c 1.1 1996/07/22 19:24:07
 *Log: mrt005.c
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
#include	"admtxt.h"			/* Show current		*/
#include	"pwbcfg.h"			/*	"		*/

#define		BUF_MAX		512
static unsigned char		*ad_phy = (void *)0x1000;

/*----------------------------------------------------------------------*/
/*	mon_dad_memrd							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCの内部ﾒﾓﾘｰのﾘｰﾄﾞを行う。					*/
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
mon_dad_memrd(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	char		store[BUF_MAX];		/* ﾘｰﾄﾞ結果のｽﾄｱｰ先	*/
	char		*mem = (char *)ad_phy;	/* ﾘｰﾄﾞするﾒﾓﾘｰへのﾎﾟｲﾝﾀ*/
	short		byte = (short)BUF_MAX;	/* ﾘｰﾄﾞするﾊﾞｲﾄ数	*/
	int		len, i, j;

	switch(argc) {
	case 3:
		if((len = strlen(argv[3])) != 4) {
err_retp2n:		mon_text_snd( ILLEGAL_NO_PRM2, argl);	return(-1);
		}
		if((j = (htoi(argv[3]) * sizeof(short))) < byte)
			byte = (short)j;

	case 2:
		if((len = strlen(argv[2])) != 4) {
err_retp1n:		mon_text_snd( ILLEGAL_NO_PRM1, argl);	return(-1);
		}
		mem = (char near *)htoi(argv[2]);
		break;

	case 1:
		break;

	default:
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}

	if((len = dad_memrd( pwbid, (void *)&store[0], (short *)mem, (short)byte))<= 0) {
		if(len == 0)
			return(mon_text_snd(READ_UNEXE, (void **)argl));
		else {
			mon_text_snd( READ_FAIL, (void **)argl); return(-1);
		}
	}
	ad_phy = (unsigned char *)(mem + len/2);
	argl[0] = &store[0];
	argl[1] = &len;
	argl[2] = &mem;
	return( mon_text_snd( READ_SUCES, (void **)argl) );
}
