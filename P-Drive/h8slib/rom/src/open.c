/*----------------------------------------------------------------------*
 * File name	: open.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/open.c 1.1 1996/01/20 12:06:16 kabumoto Exp $
 *$Log: open.c $
 * リビジョン 1.1  1996/01/20  12:06:16  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		規格－UNIX														*/
/*		nameで指定したファイルをmodeの設定に従って、オープンする。		*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*name	: ﾌｧｲﾙのﾊﾟｽ名											*/
/*		mode	: ｱｸｾｽ許可の設定										*/
/*																		*/
/* 戻り値																*/
/*		int		: ﾌｧｲﾙ･ﾊﾝﾄﾞﾙ											*/
/*				  -1	･･･ error										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>								/* ANSI std			*/
#include	<string.h>								/*	"				*/
#include	<fcntl.h>
#include	<rom\handle.h>
#define	STORAGE
#include	<drive\iofsys.h>						/* Show drive		*/
#undef	STORAGE

static const char		*fname_tbl[] = {
	"e2rom",	"sram",		"exram",	"dpmem",
	"con",		"tty",		"lan",		"inet",
	"com",		"prn"
};
static const size_t		len_tbl[] = {
	sizeof("e2rom")-1,	sizeof("sram")-1,	sizeof("exram")-1,
	sizeof("dpmem")-1,	sizeof("con")-1,	sizeof("tty")-1,
	sizeof("lan")-1,	sizeof("inet")-1,	sizeof("art")-1,
	sizeof("prn")-1
};

int
open(
	char	*name,
	int		mode
	)
{
	int			name_siz,							/* length of name	*/
				devno, len;
	register int	i = 0;

	name_siz = strlen(name);
	do {
		if(strncmp(name, fname_tbl[i], len= len_tbl[i]) == 0) { 
			if(name_siz == len)
				devno = 0;
			else
				devno = name[len] & 0xf;
			if(iodrv_open[i] == NULL)				break;
			if(iodrv_open[i](devno, mode) < 0)		return(-1);
			return((++i << 8) | devno);
		}
	} while(++i < __KEY_MAX_-1);
	if(i == __SCI_-1)								return(0);
	return(-1);
}
