/*----------------------------------------------------------------------*
 * File name	: sscanf.c	/ romable sscanf							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/sscanf.c 1.1 1996/01/19 09:50:08 kabumoto Exp $
 *$Log: sscanf.c $
 * ���r�W���� 1.1  1996/01/19  09:50:08  kabumoto
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
/*		ROM��sscanf�B													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*string	: �ϊ��ΏۂƂȂ���ݸ޽�ւ��߲���						*/
/*		*fmt	: �����ւ��߲��											*/
/*		*arg	: �ϊ���̽ı���ւ��߲��								*/
/*																		*/
/* �߂�l																*/
/*		int		: 0		��� complete									*/
/*				  -N	��� error										*/
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