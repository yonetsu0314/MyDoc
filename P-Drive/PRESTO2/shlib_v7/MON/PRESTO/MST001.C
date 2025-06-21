/*----------------------------------------------------------------------*
 * File name	: mst001.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mst001.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst001.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mst001.c 1.1 1970/01/01
 *Log: mst001.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<custom\custom.h>		/* Show xlib		*/
#include	"admtxt.h"			/* current		*/

static unsigned char	seq_no =(unsigned char)0;/* text sequnce number	*/

/*----------------------------------------------------------------------*/
/*	mon_sampl_text							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｻﾝﾌﾟﾘﾝｸﾞ･ﾃﾞｰﾀのﾃｷｽﾄを作成する。このﾃｷｽﾄは例外的にｺﾏﾝﾄﾞ部を	*/
/*	除いたﾃｷｽﾄ本体のBCC(2btye)を付加する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf	: pointer to string buffer				*/
/*	*argl[]	: pointer to argument[]					*/
/*			argl[0]	･･･ ｻﾝﾌﾟﾘﾝｸﾞ･ﾃﾞｰﾀへのﾎﾟｲﾝﾀ		*/
/*			argl[1]	･･･ ｻﾝﾌﾟﾘﾝｸﾞ･ﾃﾞｰﾀ長へのﾎﾟｲﾝﾀ		*/
/*									*/
/* 戻り値								*/
/*	char *	: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ					*/
/*----------------------------------------------------------------------*/

char *
mon_sampl_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
	int		len;
	char		c, *p, *cp;

	cp = buf;
	p = argl[0];
	ctoh( seq_no++, cp), cp += 2;
	len = *(int *)argl[1];
	do {
		*(cp++) = (char)((*p >> 4)|'0');
		*(cp++) = (char)((*(p++) & 0x0f)|'0');
	} while(--len);
	c = check_bcc( (unsigned char *)buf, (int)(cp - buf));
	*(cp++) = (char)((c >> 4)|'0');
	*(cp++) = (char)((c & 0x0f)|'0');
	*cp = (char)'\0';
	return(cp);
}
