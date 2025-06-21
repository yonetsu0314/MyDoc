/*----------------------------------------------------------------------*
 * File name	: vfbkcpy.c	/ fmemcpy and block verify		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfbkcpy.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfbkcpy.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるメモリー間の転送を行う。fmemcpyとコン	*/
/*	パチブル。転送後ベリファイを行い、一致しない場合規定回数デ	*/
/*	ータの最初からの転送／ベリファイを繰り返し、転送を失敗した	*/
/*	場合farのヌル･ポインタを返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dst	: 転送先のﾎﾟｲﾝﾀｰ					*/
/*	src	: 転送元のﾎﾟｲﾝﾀｰ					*/
/*	len	: 転送ﾊﾞｲﾄ数						*/
/*									*/
/* 戻り値								*/
/*	far *	: dst		･･･ success				*/
/*		  NULL		･･･ fail				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/

#define	RETRY	3				/* ベリファイ回数	*/

void *
verify_blk_fmemcpy(
	void	*dst,
	void	*src,
	int	len
	)
{
	int		sts;
	unsigned char	c= (unsigned char)RETRY;

	do {
		_disable();
		memcpy(dst, src, (size_t)len);
		sts = strncmp((char *)dst, (char *)src, (size_t)len);
		_enable();
		if(sts == 0)					return(dst);
	} while(--c);
	return(NULL);
}
