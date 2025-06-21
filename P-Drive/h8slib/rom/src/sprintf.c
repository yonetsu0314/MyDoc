/*----------------------------------------------------------------------*
 * File name	: sprintf.c		/ romable sprintf						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/sprintf.c 1.1 1996/01/19 09:49:50 kabumoto Exp $
 *$Log: sprintf.c $
 * ���r�W���� 1.1  1996/01/19  09:49:50  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����																	*/
/*		�K�i�|ANSI														*/
/*		ROM��sprintf�B													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		str	: ���ݸ޽�̽ı���ւ��߲���									*/
/*		*fmt	: �����ւ��߲��											*/
/*		args	: �ϊ��ΏۂƂȂ������									*/
/*																		*/
/* �߂�l																*/
/*		int		: +N	��� length										*/
/*				  -N	��� error										*/
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