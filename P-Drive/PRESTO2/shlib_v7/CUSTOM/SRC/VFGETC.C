/*----------------------------------------------------------------------*
 * File name	: vfgetc.c	/ get char data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfgetc.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfgetc.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファの内容を1バイト読み込み、ベリフ	*/
/*	ァイし、一致したデータを返す。なお規定回数一致が見られない場	*/
/*	合エラーコードを返す。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	p	: ﾘｰﾄﾞするﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0x00??	･･･ success				*/
/*		  -1L		･･･ fail				*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/

#define	RETRY	3				/* ベリファイ回数	*/

int
verify_getc(
	volatile unsigned char	*p
	)
{
	unsigned char	i, c= (unsigned char)RETRY;

	_disable();
	do {
		i = *p;
		if(i == *p) {
			_enable();			return((int)i);
		}
	} while(--c);
	_enable();
	return(-1);
}
