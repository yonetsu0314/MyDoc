/*----------------------------------------------------------------------*
 * File name	: mst007.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mst007.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mst007.c 1.1 1996/07/22 19:25:59
 *Log: mst007.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<stdlib.h>			/* ANSI std		*/
#include	<custom\custom.h>		/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

/*----------------------------------------------------------------------*/
/*	mon_wei_text							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	計量器データののﾃｷｽﾄを作成する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf	: pointer to string buffer				*/
/*	*argl[]	: pointer to argument[]					*/
/*			argl[0]	･･･ 実行結果ﾋﾞｯﾄ･ﾊﾟﾀｰﾝへのﾎﾟｲﾝﾀ		*/
/*			argl[1]	･･･ 零A/Dﾃﾞｰﾀ[24]へのﾎﾟｲﾝﾀ		*/
/*			argl[2]	･･･ 零D/Aﾃﾞｰﾀ[24]へのﾎﾟｲﾝﾀ		*/
/*									*/
/* 戻り値								*/
/*	char *	: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ					*/
/*----------------------------------------------------------------------*/


char *
mon_wei_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
	struct dades10	es;			/* error status		*/
	char		*cp, *p;
	unsigned short	k;
	int		len, i, j, m;

/*
	es = argl[0];
*/
/*	len = strlen(buf);*/
/*	cp = buf+len;*/
	cp = buf;
/*
	*(cp++) = '\n';
	*(cp++) = '\r';
	p = argl[1];
	i = 8;
	for(m =0 ; m <(DAD_WEI_MAX +DAD_AFV_MAX +DAD_DF_MAX) ; m++) {
		if(i == 8)  *(cp++) = '\t';
		itoh(*(int *)p, cp);
		p += 2;
		cp += 4;
		if(--i) *(cp++) = ' ';
		else {
			*(cp++) = '\n';
			*(cp++) = '\r';
			i = 8;
		}
	}
*/
/*
	*(cp++) = '\n';
	*(cp++) = '\r';
*/
	p = argl[2];
	for(m =0 ; m <(DAD_WEI_MAX +DAD_AFV_MAX +DAD_DF_MAX) ; m++) {
		itoh(*(unsigned short *)p, cp);
		p += 2;
		len = strlen(buf);
		cp = (char *)(buf+len);
		*(cp++) =(char) ' ';
	}
	*cp = (char)'\0';
	return(cp);
}
