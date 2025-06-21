/*----------------------------------------------------------------------*
 * File name	: printf.c	/ romable printf							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/printf.c 1.1 1996/01/19 09:46:22 kabumoto Exp $
 *$Log: printf.c $
 * リビジョン 1.1  1996/01/19  09:46:22  kabumoto
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
/*		ROM化printf。													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*fmt	: 書式へのﾎﾟｲﾝﾀ											*/
/*		args	: 変換対象となる引き数									*/
/*																		*/
/* 戻り値																*/
/*		int		: 0		･･･ complete									*/
/*				  -N	･･･ error										*/
/*----------------------------------------------------------------------*/

#include	<stdarg.h>								/* ANSI std			*/
#include	<stdio.h>								/*	"				*/

int		putch(int);
int		format(int(*func)(int), char *fmt, va_list ap);

int
printf(const char *fmt, ...)
{
	va_list		ap;
	int 		i;

	va_start(ap, fmt);
	i= format(putch, fmt, ap);
	va_end(ap);
	return(i);
}
