/*----------------------------------------------------------------------*
 * File name	: open.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/rcs/open.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: open.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * fifo定義追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/02/17 18:00
 * fromを追加する。
 * 
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
/*	規格－UNIX							*/
/*	nameで指定したファイルをmodeの設定に従って、オープンする。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*name	: ﾌｧｲﾙのﾊﾟｽ名						*/
/*	mode	: ｱｸｾｽ許可の設定					*/
/*									*/
/* 戻り値								*/
/*	int	: ﾌｧｲﾙ･ﾊﾝﾄﾞﾙ						*/
/*		  -1		･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rom\fcntl.h>			/* MS-C std		*/
#include	<rom\handle.h>			/* Show xlib		*/
#define	STORAGE
#include	<drive\iofsys.h>		/* Show drive		*/
#undef	STORAGE

static const unsigned char	*fname_tbl[] = {
	"e2rom",	"sram",		"from",		"exram",
	"dpmem",	"con",		"tty",		"lan",
	"inet",		"com",		"prn",		"fifo"
};
static const size_t		len_tbl[] = {
	sizeof("e2rom")-1,	sizeof("sram")-1,	sizeof("from")-1,
	sizeof("exram")-1,	sizeof("dpmem")-1,	sizeof("con")-1,
	sizeof("tty")-1,	sizeof("lan")-1,	sizeof("inet")-1,
	sizeof("com")-1,	sizeof("prn")-1,	sizeof("fifo")-1
};

int
open(
	unsigned char	*name,
	int		mode
	)
{
	int	name_siz,		/* length of name	*/
		devno, len;
	int	i = 0;

	name_siz = strlen((char *)name);
	do {
		if(strncmp((char *)name, (char *)fname_tbl[i], (size_t)(len= len_tbl[i])) == 0) { 
			if(name_siz == len)
				devno = 0;
			else
				devno = name[len] & 0xf;
			if(iodrv_open[i] == NULL)		break;
			if(iodrv_open[i](devno, mode) < 0)	return(-1);
			return((++i << 8) | devno);
		}
	} while(++i < __KEY_MAX_-1);
	if(i == __SCU_-1)					return(0);
	return(-1);
}
