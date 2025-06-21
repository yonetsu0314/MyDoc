/*----------------------------------------------------------------------*
 * File name	: redirect.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/redirect.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: redirect.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	規格－ISHIDA							*/
/*	nameで指定したファイルを標準入出力とする			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	fd	: 対象ﾌｧｲﾙのﾊﾝﾄﾞﾙ					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rom\fcntl.h>			/* MS-C std		*/
#include	<rom\handle.h>			/* Show xlib		*/
#include	<drive\iofsys.h>		/* Show drive		*/

int	stdio_filno = 0;
int	stdio_devno = 0;

int
redirect(
	int		fd
	)
{
	int		i;

	stdio_devno = fd & 0xff;
	if(((i= ((fd & 0xff00) >> 8)-1) < 0) || (i > __KEY_MAX_-1) ||
	  (iodrv_open[i] == NULL))				return(-1);
	stdio_filno = i;
	return(0);
}
