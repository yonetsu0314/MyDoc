/*----------------------------------------------------------------------*
 * File name	: mst006.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mst006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mst006.c 1.1 1996/07/22 19:25:14
 *Log: mst006.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<custom\custom.h>		/* custom		*/
#include	<string.h>			/* ANSI std		*/
#include	"admtxt.h"			/* current		*/

/*----------------------------------------------------------------------*/
/*	mon_showmap_text						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARCNET接続状況のﾃｷｽﾄを作成する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf		: pointer to string buffer			*/
/*	*argl[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	char *		: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ				*/
/*----------------------------------------------------------------------*/

char *
mon_showmap_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
	char		*cp;
	unsigned char	*p;
	int		add_h, add_l, len;

	len = strlen(buf);
	cp = (char *)(buf+len);
	*(cp++) = (char)'\n';
	*(cp++) = (char)'\r';
	p = argl[0];
	*(cp++) = (char)0x20;
	for ( add_h=0; add_h<16; add_h++ ) {
		*(cp++) = hex_ascii(add_h);
	}
	*(cp++) = (char)'\n';
	*(cp++) = (char)'\r';
	for ( add_h=0; add_h<16; add_h++ ) {
		*(cp++) = hex_ascii(add_h);
		for ( add_l=0; add_l<16; add_l++ ) {
			*(cp++) = (*(char *)(p+add_l+add_h*16))? (char)'*' : (char)'-' ;
		}
		*(cp++) = (char)'\n';
		*(cp++) = (char)'\r';
	}
	*cp = (char)'\0';
	return(cp);
}
