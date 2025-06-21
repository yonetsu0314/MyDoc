/*----------------------------------------------------------------------*
 * File name	: redirect.c											*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/redirect.c 1.1 1996/01/20 12:06:32 kabumoto Exp $
 *$Log: redirect.c $
 * リビジョン 1.1  1996/01/20  12:06:32  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		規格－ISHIDA													*/
/*		nameで指定したファイルを標準入出力とする						*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		fd		: 対象ﾌｧｲﾙのﾊﾝﾄﾞﾙ										*/
/*																		*/
/* 戻り値																*/
/*		int		: 0		･･･ complete									*/
/*				  -1	･･･ error										*/
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