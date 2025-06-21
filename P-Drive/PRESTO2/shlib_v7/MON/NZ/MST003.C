/*----------------------------------------------------------------------*
 * File name	: mst003.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mst003.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst003.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mst003.c 1.1 1996/07/22 19:25:23
 *Log: mst003.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

/*----------------------------------------------------------------------*/
/*	mon_memrd_text							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾒﾓﾘｰ･ﾘｰﾄﾞ完了のﾃｷｽﾄを作成する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf		: pointer to string buffer			*/
/*	*argl[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	char *		: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ				*/
/*----------------------------------------------------------------------*/

char *
mon_memrd_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
	char		*cp;
	unsigned short	*p;
	int		add, len, i, m;

	len = strlen(buf);
	cp = (char *)(buf+len);
	*(cp++) = (char)'\n';
	*(cp++) = (char)'\r';
	p = argl[0];
	len = *(int *)argl[1] /sizeof(short);
	add = *(int *)argl[2];
	i = 8;
	for(m =0; m < len ; m++) {
		if(i == 8) {
			*(cp++) = (char)'\t';
			itoh((unsigned short)add, cp);
			cp += sizeof(short) *2;
			add += 8;
			*(cp++) = (char)' ';
			*(cp++) = (char)':';
			*(cp++) = (char)' ';
		}
		itoh(*(p++), cp);
		cp += sizeof(short) *2;
		if(--i) *(cp++) = (char)' ';
		else {
			*(cp++) = (char)'\n';
			*(cp++) = (char)'\r';
			i = 8;
		}
	}
	*cp = (char)'\0';
	return(cp);
}
