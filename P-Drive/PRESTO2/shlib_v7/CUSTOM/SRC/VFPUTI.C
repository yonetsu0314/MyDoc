/*----------------------------------------------------------------------*
 * File name	: vfputi.c	/ put word data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfputi.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfputi.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファの内容にshortデータを書き込み後	*/
/*	ベリファイし、一致するまで規定回数繰り返す。なお一致が見られ	*/
/*	ない場合エラーコードを返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dat	: 書き込みを行うshort型ﾃﾞｰﾀ				*/
/*	p	: shortへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ success					*/
/*		  -1	･･･ fail					*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/

#define	RETRY	3				/* ベリファイ回数	*/

int
verify_puti(
	unsigned short	dat,
	volatile unsigned short	*p
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
