/*----------------------------------------------------------------------*
 * File name	: redirect.c											*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/redirect.c 1.1 1996/01/20 12:06:32 kabumoto Exp $
 *$Log: redirect.c $
 * ���r�W���� 1.1  1996/01/20  12:06:32  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����																	*/
/*		�K�i�|ISHIDA													*/
/*		name�Ŏw�肵���t�@�C����W�����o�͂Ƃ���						*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		fd		: �Ώ�̧�ق������										*/
/*																		*/
/* �߂�l																*/
/*		int		: 0		��� complete									*/
/*				  -1	��� error										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>								/* ANSI std			*/
#include	<string.h>								/*	"				*/
#include	<fcntl.h>
#include	<rom\handle.h>
#include	<drive\iofsys.h>

int		stdio_filno = 0;
int		stdio_devno = 0;

int
redirect(
	int		fd
	)
{
	int		i;

	stdio_devno = fd & 0xff;
	if(((i= ((fd & 0xff00) >> 8)-1) < 0) || (i > __KEY_MAX_-1) ||
	  (iodrv_open[i] == NULL))							return(-1);
	stdio_filno = i;
	return(0);
}
