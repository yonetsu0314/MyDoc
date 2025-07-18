/*----------------------------------------------------------------------*
 * File name	: mrt018.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt018.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt018.c 1.1 1996/07/22 19:25:46
 *Log: mrt018.c
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
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
/*#include	"bkf004.h"			/* current		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

#define MAX_NO	(DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX)
#define	HEAD_MAX	MAX_NO

/*----------------------------------------------------------------------*/
/*	mon_dad_weird							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	計量器データを読み出す						*/
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
mon_dad_weird(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	DAD_ARY_PTN	xs =0;			/* 指定ﾍｯﾄﾞ･ﾊﾟﾀｰﾝ	*/
/*	DAD_ARY_PTN	exe_bp[2];		/* 実行結果  "		*/
/*	short		err_sts[sizeof(xs)*CHAR_BIT];			*/
	int		len, err, i, n, j;
	char		*cp, *pp;

	unsigned short	s3[HEAD_MAX],		/* 3段目ｽﾃｰﾀｽ		*/
			*ps3 = s3,		/*	"    へのﾎﾟｲﾝﾀ	*/
			s4[HEAD_MAX],		/* 4段目ｽﾃｰﾀｽ		*/
			*ps4 = s4;		/* 	"    へのﾎﾟｲﾝﾀ	*/
	short		d3[HEAD_MAX],		/* 3段目data		*/
			*pd3 = d3,		/* 	"    へのﾎﾟｲﾝﾀ	*/
			d4[HEAD_MAX],		/* 4段目data		*/
			*pd4 = d4;		/* 	"    へのﾎﾟｲﾝﾀ	*/
	struct dades10	es;			/* error status		*/

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
		xs |= 0x1L << (j-1);
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
	argl[0] = &es;
	argl[1] = pd3;
	argl[2] = pd4;
	if(err = dad_wei_read( pwbid, &es, ps3, pd3, ps4, pd4) == -1) {
		mon_text_snd( WEI_READ_FAIL, argl);
		return(-1);
	}
	return( mon_text_snd( WEI_READ_SUCES, argl) );
}
