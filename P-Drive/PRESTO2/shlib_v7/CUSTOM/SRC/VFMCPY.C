/*----------------------------------------------------------------------*
 * File name	: vfmcpy.c	/ fmemcpy and verify			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/vfmcpy.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfmcpy.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ポインタで示されるメモリー間の転送を行う。memcpyとコンパチ	*/
/*	ブル。転送後ベリファイを行い、一致しない場合規定回数転送／	*/
/*	ベリファイを繰り返し、転送を失敗した場合farのヌル･ポインタ	*/
/*	を返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dst	: 転送先のfarﾎﾟｲﾝﾀｰ					*/
/*	src	: 転送元のfarﾎﾟｲﾝﾀｰ					*/
/*	len	: 転送ﾊﾞｲﾄ数						*/
/*									*/
/* 戻り値								*/
/*	far *	: dst		･･･ success				*/
/*		  NULL		･･･ fail				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<custom\xmemory.h>		/* show xlib		*/

#define	RETRY	3				/* ベリファイ回数	*/

typedef union {
	unsigned char	c[2];			/* charの配列データ	*/
	short		s;			/* shortのデータ	*/
} UCS;						/* コピーデータの構造	*/

void *
verify_fmemcpy(
	void	*dst,
	void	*src,
	int	len
	)
{
	volatile UCS	*source;		/* ソースアドレス	*/
	volatile UCS	*dest;			/* 転送先アドレス	*/
	int		n;			/* 転送word数		*/
	int		odd;			/* 奇数バイトか否か	*/
	unsigned char	c= (unsigned char)RETRY;

	odd = len & 0x1;
	do {
		memcpy(dst, src, (size_t)len);

		source = src;
		dest = dst;
		if(n = (len >> 1)) {
			do {
				if(dest->s != source->s) {
					dest->s= source->s;
					if(dest->s != source->s)  break;
				}
				++source, ++dest;
			} while(--n);
			if(n)  continue;
		}
		if(odd) {
			if(dest->c[0] != source->c[0]) {
				dest->c[0]= source->c[0];
				if(dest->c[0] != source->c[0])  continue;
			}
		}
		return((void *)dst);
	} while(--c);
	return(NULL);
}
