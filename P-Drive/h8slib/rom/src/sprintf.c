/*----------------------------------------------------------------------*
 * File name	: sprintf.c		/ romable sprintf						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/sprintf.c 1.1 1996/01/19 09:49:50 kabumoto Exp $
 *$Log: sprintf.c $
 * リビジョン 1.1  1996/01/19  09:49:50  kabumoto
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
/*		ROM化sprintf。													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		str	: ｽﾄﾘﾝｸﾞｽのｽﾄｱｰ先へのﾎﾟｲﾝﾀｰ									*/
/*		*fmt	: 書式へのﾎﾟｲﾝﾀ											*/
/*		args	: 変換対象となる引き数									*/
/*																		*/
/* 戻り値																*/
/*		int		: +N	･･･ length										*/
/*				  -N	･･･ error										*/
/*----------------------------------------------------------------------*/

#include	<stdarg.h>								/* ANSI std			*/
#include	<stdio.h>								/*	"				*/

int			format(int(*func)(int), char *fmt, va_list ap);
static char		*buff;

static int	spsub(int c)
{
	return (*buff++ = c)&0xff;
}

int
sprintf(char *str, const char *fmt, ...)
{
	va_list		ap;
	int 		i;

	buff = str;
	va_start(ap, fmt);
	i= format(spsub, fmt, ap);
	va_end(ap);
	*buff = 0;
	return(i);
}
