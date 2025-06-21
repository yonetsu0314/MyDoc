/*----------------------------------------------------------------------*
 * File name	: sscanf.c	/ romable sscanf							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/sscanf.c 1.1 1996/01/19 09:50:08 kabumoto Exp $
 *$Log: sscanf.c $
 * リビジョン 1.1  1996/01/19  09:50:08  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		規格－ANSI														*/
/*		ROM化sscanf。													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*string	: 変換対象となるｽﾄﾘﾝｸﾞｽへのﾎﾟｲﾝﾀｰ						*/
/*		*fmt	: 書式へのﾎﾟｲﾝﾀ											*/
/*		*arg	: 変換後のｽﾄｱｰ先へのﾎﾟｲﾝﾀ								*/
/*																		*/
/* 戻り値																*/
/*		int		: 0		･･･ complete									*/
/*				  -N	･･･ error										*/
/*----------------------------------------------------------------------*/

#include	<limits.h>									/* ANSI std		*/

static char 		*scnstr;
static char 		quit;

static int
sgetc(int what)
{
	if (what == 0) {
		if (*scnstr)
			return *scnstr++ & 255;
		quit = 1;
	} else {
		if (!quit)
			return *--scnstr & 255;
	}
	return -1;
}

sscanf(
	char		*string,
	char		*fmt,
	int 		*arg
	)
{
	int		i;


	scnstr = string;
	quit = 0;
	i = scanfmt(sgetc, fmt, &arg);
	return(i);
}
