/*----------------------------------------------------------------------*
 * File name	: vfbitrd.c	/ read bit data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfbitrd.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfbitrd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファをビット単位で読み込みを行い、	*/
/*	ベリファイ後、一致したデータを返す。なお規定回数一致が見ら	*/
/*	れない場合エラーコードを返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	msk	: 8bitﾏｽｸ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ					*/
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
verify_bitrd(
	unsigned char		msk,
	volatile unsigned char	*p
	)
{
	unsigned char	i, j, c= (unsigned char)RETRY;

	_disable();
	do {
		i = j = *p;
		if(((i ^ *p) & msk) == 0) {
			_enable();			return (int)(j & msk);
		}
	} while(--c);
	_enable();
	return(-1);
}
