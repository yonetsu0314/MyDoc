/*----------------------------------------------------------------------*
 * File name	: vfgeti.c	/ get word data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfgeti.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfgeti.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファの内容をshort(2バイトデータ)		*/
/*	で読み込み後ベリファイし、一致したデータをlong型にキャス	*/
/*	トして返す。なお規定回数一致が見られない場合エラーコード	*/
/*	を返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	p	: ﾘｰﾄﾞするﾊﾞｯﾌｧへのfarﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	long	: 0x0000????	･･･ success				*/
/*		  -1L		･･･ fail				*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/

#define	RETRY	3				/* ベリファイ回数	*/

long
verify_geti(
	volatile unsigned short	*p
	)
{
	unsigned short	i;
	unsigned char	c= (unsigned char)RETRY;

	_disable();
	do {
		i = *p;
		if(i == *p) {
			_enable();			return (long)i;
		}
	} while(--c);
	_enable();
	return(-1L);
}
