/*----------------------------------------------------------------------*
 * File name	: sprintf.c	/ romable sprintf			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/sprintf.c 1.5 1970/01/01 00:00:00 chimura Exp $
 *$Log: sprintf.c $
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/10/06 15:00
 * 割り込み禁止状態で関数をぬける不具合修正
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/09/16　18:15
 * 前リビジョンの不具合修正
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/16 18;15
 * 書式を２個以上指定できない不具合修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/05/01 20:30
 * 二重定義を回避する。
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
/*	ﾏﾙﾁ･ﾀｽｸ対応のROM化sprintf。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	str	: ｽﾄﾘﾝｸﾞｽのｽﾄｱｰ先へのﾎﾟｲﾝﾀｰ				*/
/*	*fmt	: 書式へのﾎﾟｲﾝﾀ						*/
/*	args	: 変換対象となる引き数					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -N	･･･ error					*/
/*----------------------------------------------------------------------*/

#include	<stdarg.h>			/* ANSI std		*/
#include	<stddef.h>			/* ANSI std		*/
#include	<limits.h>			/* ANSI std		*/
/*#include	<dos.h>*/			/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<custom\custom.h>

extern int format(int (*putsub)(int), char *fmt, char *argp);

static union aap	print_sem = { 0L };	/* ｾﾏﾌｫ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
static unsigned char	nest_cnt = (unsigned char)0;	/* ﾈｽﾃｨﾝｸﾞ回数		*/
static char		*buff;

static int
spsub(int c)
{
	return ((int)(*buff++ = (char)c)&0xff);
}

int
sprintf(
	char		*str,
	const char	*fmt,
	...
	)
{
	int 		i, err =0;
	va_list		ap;
	unsigned	args[16];

	_disable();
	if(print_sem.acadr == 0) {
		if(CRE_SEM( FIFO, &print_sem.acadr,
			    SPRINT_SEM_ID, 1))			return(-1);
		if(WAI_SEM( ENDLESS, print_sem.acadr,
			    1, VOID_TIME))			return(-1);
	}
	_enable();
	ADV_WAI_SEM(err, nest_cnt, ENDLESS, print_sem.acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);

	buff = str;
	va_start(ap,fmt);
	for(i=0; i<16; i++){
		args[i] = va_arg(ap,int);
	}
	i = format( spsub, fmt, (char *)&args[0]);
	va_end(ap);
	*buff = 0;

	ADV_SIG_SEM(err, nest_cnt, print_sem.acadr);
	if(err)							return(err|INT_MIN);
	return(i);
}
