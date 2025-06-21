/*----------------------------------------------------------------------*
 * File name	: putchar.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/putchar.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: putchar.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/*
	putchar(c);
*/
extern int	putch(int chr);

void putchar(c)
	char	c;
{
	putch((int)c);
}
