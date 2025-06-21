/*----------------------------------------------------------------------*
 * File name	: mrt006.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt006.c 1.1 1996/07/22 19:25:11
 *Log: mrt006.c
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
#include	"pwbcfg.h"			/*	"		*/

#define		BUF_MAX		512

/*----------------------------------------------------------------------*/
/*	mon_dad_memwr							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCの内部ﾒﾓﾘｰにﾗｲﾄを行う。					*/
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
mon_dad_memwr(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	short		*mem;			/* ﾗｲﾄするﾒﾓﾘｰへのﾎﾟｲﾝﾀ	*/
	char		load[BUF_MAX];		/* 書き込みﾃﾞｰﾀ		*/
	short		byte =(short)0;		/* ﾘｰﾄﾞするﾊﾞｲﾄ数	*/
	int		len, i, j;
	char		*cp, *pp;

	if(argc != 3) {
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}
	if((len = strlen(argv[2])) != 4) {
		mon_text_snd( ILLEGAL_NO_PRM1, argl);		return(-1);
	}
	mem = (short *)htoi(argv[2]);

	if((len = strlen(argv[3])) > 1279)  goto err_retp3n;
	pp = cp = argv[3];
	while((cp = strchr( pp, PUNCTUATOR)) != NULL) {
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) != 4)  goto err_retp3n;
		pp += sizeof(short);
		for(i = sizeof(short)-1 ; i >=0  ; i--) {
			if((j = htoc(pp)) > 0xff)  goto err_retp3;
			pp -= sizeof(short);
			load[byte++] = (char)j;
		}
		pp = cp;
		if(byte >= BUF_MAX/sizeof(short))  goto err_retp3;
	}
	if((len = strlen(pp)) != 4) {
err_retp3n:	mon_text_snd( ILLEGAL_NO_PRM3, argl);		return(-1);
	}
	pp += sizeof(short);
	for(i = sizeof(short)-1 ; i >=0  ; i--) {
		if((j = htoc(pp)) > 0xff) {
err_retp3:		mon_text_snd( ILLEGAL_PRM3, argl);	return(-1);
		}
		pp -= sizeof(short);
		load[byte++] = (char)j;
	}

	if((len = dad_memwr( pwbid, (char *)mem, &load[0], (short)byte))<= 0) {
		if(len == 0)
			return( mon_text_snd( WRITE_UNEXE, argl) );
		else {
			mon_text_snd( WRITE_FAIL, argl);	return(-1);
		}
	}
	return( mon_text_snd( WRITE_SUCES, argl) );
}
