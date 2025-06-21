/*----------------------------------------------------------------------*
 * File name	: printf.c	/ romable printf			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/printf.c 1.6 1970/01/01 00:00:00 chimura Exp $
 *$Log: printf.c $
 * リビジョン 1.6  1970/01/01  00:00:00  chimura
 * 2004/01/29 15:00
 * ADV_SIG_SEMへの引き数の誤りを修正する。
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/10/06 15:00
 * 割り込み禁止状態で関数をぬける不具合修正
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/09/16 18:15
 * 前リビジョンの不具合修正
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/16 18:15
 * 書式を２個以上指定できない不具合修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/05/01 20:30
 * 可変引数に書きなおす。
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
/*	ﾏﾙﾁ･ﾀｽｸ対応のROM化printf。					*/
/*   注)								*/
/*	printfの1回の関数呼出に対して排他制御を行っているため、APL側	*/
/*	で1ﾃｷｽﾄを複数回のprintfで記述している場合は保証出来ない。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
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
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<custom\custom.h>

extern int format(int (*putsub)(int), char *fmt, char *argp);

/* int	putch(void *); */
extern int	putch(int chr);

static union aap	print_sem = { 0L };	/* ｾﾏﾌｫ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
static unsigned char	nest_cnt = (unsigned char)0;	/* ﾈｽﾃｨﾝｸﾞ回数		*/

int
printf(
	const char		*fmt,
	...
	)
{
	int		i,err =0;
	va_list		ap;
	unsigned	args[16];

	_disable();
	if(print_sem.acadr == 0) {
		if(CRE_SEM( FIFO, &print_sem.acadr,
			    (unsigned int)PRINT_SEM_ID, (unsigned int)1))	return(-1);
		if(WAI_SEM( ENDLESS, print_sem.acadr,
			    (unsigned int)1, VOID_TIME))			return(-1);
	}
	_enable();
	ADV_WAI_SEM(err, nest_cnt, ENDLESS, print_sem.acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);

	va_start(ap,fmt);
	for(i=0; i<16; i++){
		args[i] = va_arg(ap,int);
	}
	format(putch, fmt, (char *)&args);
	va_end(ap);

	ADV_SIG_SEM(err, nest_cnt, print_sem.acadr);
	if(err)							return(err|INT_MIN);
	return(0);
}
