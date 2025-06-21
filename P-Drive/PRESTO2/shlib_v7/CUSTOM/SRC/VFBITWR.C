/*----------------------------------------------------------------------*
 * File name	: vfbitwr.c	/ write bit data and verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfbitwr.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfbitwr.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるバッファに対しビット･セット／リセットを	*/
/*	行なった後ベリファイする。なお規定回数一致が見られない場合エ	*/
/*	ラーコードを返す。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ptn	: 8bitﾋﾞｯﾄ･ﾊﾟﾀｰﾝﾃﾞｰﾀ					*/
/*	set	: ｾｯﾄ:1かﾘｾｯﾄ:0の指示					*/
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
verify_bitwr(
	unsigned char		ptn,
	int			set,
	volatile unsigned char	*p
	)
{
	unsigned char	dat, c= (unsigned char)RETRY;

	if(set) {				/* ﾋﾞｯﾄ･ｾｯﾄﾓｰﾄﾞ		*/
		_disable();
		do {
			*p = (dat = (unsigned char)(*p | ptn));
			if(*p == dat) {
				_enable();			return(0);
			}
		} while(--c);
		_enable();
	}
	else {					/* ﾋﾞｯﾄ･ﾘｾｯﾄﾓｰﾄﾞ	*/
		ptn = (unsigned char)(~ptn);
		_disable();
		do {
			*p = (dat = (unsigned char)(*p & ptn));
			if(*p == dat) {
				_enable();			return(0);
			}
		} while(--c);
		_enable();
	}
	return(-1);
}
