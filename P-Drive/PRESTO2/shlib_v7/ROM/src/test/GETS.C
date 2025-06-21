/*----------------------------------------------------------------------*
 * File name	: gets.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/gets.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: gets.c $
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
	n = gets(s);
		char	*s;

	read a string of characters
	from stdin input stream,
	into the buffer pointed by s.
	gets doesn't put <CR> in the 
	caller's buffer.
	return read character number
	if ESC key read,return -1

*/

#define	_LF	0x0a
#define	_CR	0x0d
#define	_BS	0x08
#define	_ESC	0x1b
#define	_DEL	0x7f
extern int getch(void);
extern int putch(int);

char *gets(s)
	char	*s;
{
	int	cnt;
	char	c;
	cnt = 0;

	while(1)
	{
	c = (char)getch();
	switch(c)
		{
		case _CR:	*s = (char)0;
				putch('\n');
				putch('\r');
				return((char *)cnt);
/*				break; */

		case _DEL:	c = (char)_BS;
		case _BS:	if(cnt != 0)
				{
				putch(_BS); putch(' '); putch(_BS);
				--cnt;
				--s;
				}
				break;

		case _ESC:	*s = (char)0;
				return((char *)-1);
/*				break; */
	
		default:	if(c > ' ')
				{
				*s++ = c;
				putch((int)c);
				++cnt;
				}
				break;
		}
	}

}

