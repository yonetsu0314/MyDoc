/*----------------------------------------------------------------------*
 * File name	: puts.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/puts.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: puts.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
#include	<stdio.h>			/* ANSI std		*/

/*
	puts(s);
		char	*s;
	puts writes the null-terminated
	strings to the stdio stream
	and then an end-of-line sequence.
	It returns a non-negative value,
	if no errors occur.
*/
extern int cputs(const char *);
extern int putch(int);

int puts(s)
	const char	*s;
{
	if(cputs(s) != -1)
		{ 	putch('\n');
			putch('\r');
			return(0);
		}
	return(-1);
}
