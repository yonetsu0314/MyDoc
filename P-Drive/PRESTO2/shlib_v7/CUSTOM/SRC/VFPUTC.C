/*----------------------------------------------------------------------*
 * File name	: vfputc.c	/ put char data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfputc.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfputc.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファの内容に1バイト書き込み後、ベリ	*/
/*	ファイし、一致するまで規定回数繰り返す。なお一致が見られない	*/
/*	場合エラーコードを返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dat	: 書き込みを行うchar型ﾃﾞｰﾀ				*/
/*	p	: charへのﾎﾟｲﾝﾀ						*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ success					*/
/*		  -1	･･･ fail					*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/

#define	RETRY	3				/* ベリファイ回数	*/

int
verify_putc(
	unsigned char		dat,
	volatile unsigned char	*p
	)
{
	unsigned char	c= (unsigned char)RETRY;

	_disable();
	do {
		*p = dat;
		if(*p == dat) {
			_enable();				return(0);
		}
	} while(--c);
	_enable();
	return(-1);
}
