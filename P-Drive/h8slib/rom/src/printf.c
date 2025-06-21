/*----------------------------------------------------------------------*
 * File name	: printf.c	/ romable printf							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/printf.c 1.1 1996/01/19 09:46:22 kabumoto Exp $
 *$Log: printf.c $
 * ���r�W���� 1.1  1996/01/19  09:46:22  kabumoto
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
/*		ROM��printf�B													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*fmt	: �����ւ��߲��											*/
/*		args	: �ϊ��ΏۂƂȂ������									*/
/*																		*/
/* �߂�l																*/
/*		int		: 0		��� complete									*/
/*				  -N	��� error										*/
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