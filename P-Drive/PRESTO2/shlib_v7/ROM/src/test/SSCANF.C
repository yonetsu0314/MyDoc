/*----------------------------------------------------------------------*
 * File name	: sscanf.c	/ romable sscanf			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/sscanf.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: sscanf.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/10/06 15:00
 * 割り込み禁止状態で関数をぬける不具合修正
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/16 18:15
 * 前リビジョンの不具合修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/09/16　18:15
 * 書式を２個以上指定できない不具合修正
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
/*	規格－ANSI							*/
/*	ﾏﾙﾁ･ﾀｽｸ対応のROM化sscanf。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*string	: 変換対象となるｽﾄﾘﾝｸﾞｽへのﾎﾟｲﾝﾀｰ			*/
/*	*fmt	: 書式へのﾎﾟｲﾝﾀ						*/
/*	*arg	: 変換後のｽﾄｱｰ先へのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -N	･･･ error					*/
/*----------------------------------------------------------------------*/

#include	<stdarg.h>			/* ANSI std		*/
#include	<limits.h>			/* ANSI std		*/
/*#include	<dos.h>*/			/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<custom\custom.h>

extern int scanfmt(int (*getsub)(int), char *fmt, int **args);

static union aap	scan_sem = { 0L };	/* ｾﾏﾌｫ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
static unsigned char	nest_cnt = (unsigned char)0;	/* ﾈｽﾃｨﾝｸﾞ回数		*/
static char 		*scnstr;
static char 		quit;

static int
sgetc(int what)
{
	if (what == 0) {
		if (*scnstr)
			return *scnstr++ & 255;
		quit = (char)1;
	} else {
		if (!quit)
			return *--scnstr & 255;
	}
	return -1;
}

int
sscanf(
	char		*string,
	char		*fmt,
	...
	)
{
	int		i, err =0;
	va_list		ap;
	unsigned	args[16];

	_disable();
	if(scan_sem.acadr == 0) {
		if(CRE_SEM( FIFO, &scan_sem.acadr,
			    (unsigned int)SSCAN_SEM_ID, (unsigned int)1))	return(-1);
		if(WAI_SEM( ENDLESS, scan_sem.acadr,
			    (unsigned int)1, VOID_TIME))			return(-1);
	}
	_enable();
	ADV_WAI_SEM(err, nest_cnt, ENDLESS, scan_sem.acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);

	scnstr = string;
	quit = (char)0;
	va_start(ap,fmt);
	for(i=0; i<16; i++){
		args[i] = va_arg(ap,int);
	}
	i = scanfmt(sgetc, fmt, (int **)&args);
	va_end(ap);

	ADV_SIG_SEM(err, nest_cnt, scan_sem.acadr);
	if(err)							return(err|INT_MIN);
	return(i);
}
