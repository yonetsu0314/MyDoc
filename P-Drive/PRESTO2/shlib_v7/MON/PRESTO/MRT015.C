/*----------------------------------------------------------------------*
 * File name	: mrt015.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt015.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt015.c 1.1 1970/01/01
 *Log: mrt015.c
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
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

#define		BUF_MAX		512
static unsigned char		*dsp_phy = (void *)0x1000;

/*----------------------------------------------------------------------*/
/*	mon_dsp_memrd							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	DSPの内部ﾒﾓﾘｰのﾘｰﾄﾞを行う。					*/
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
mon_dsp_memrd(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	char		store[BUF_MAX];		/* ﾘｰﾄﾞ結果のｽﾄｱｰ先	*/
	char		*mem = (char *)dsp_phy;	/* ﾘｰﾄﾞするﾒﾓﾘｰへのﾎﾟｲﾝﾀ*/
	short		byte = (short)BUF_MAX;	/* ﾘｰﾄﾞするﾊﾞｲﾄ数	*/
	int		len, i, j;
	char		tmp;

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
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}

	if((len = dad_dsp_memrd( pwbid, (void *)store, (short *)mem, byte)) <= 0) {
		if(len == 0)
			return( mon_text_snd( DSP_READ_UNEXE, argl) );
		else {
			mon_text_snd( DSP_READ_FAIL, argl );	return(-1);
		}
	}
	for(i=0; i<BUF_MAX/2; i++){
		tmp          = store[2*i];
		store[i*2]   = store[i*2+1];
		store[i*2+1] = tmp;
	}
	dsp_phy = (unsigned char *)(mem + len/2);
	argl[0] = &store[0];
	argl[1] = &len;
	argl[2] = &mem;
	return( mon_text_snd( DSP_READ_SUCES, argl) );
}
