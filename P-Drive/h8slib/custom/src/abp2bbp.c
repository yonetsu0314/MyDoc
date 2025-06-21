/*----------------------------------------------------------------------*
 * File name	: abp2bbp.c		/ ascii ptn to binary ptn convert		*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/custom/src/RCS/abp2bbp.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: abp2bbp.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993-2000, ISHIDA  CO., LTD.				*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		abpで示される指定バイト数のASCIIビットパターン(abp[0]のbit0		*/
/*		がLSB)をバイナリーのビットパターンに変換し32bitで結果を返す。	*/
/*																		*/
/*      注） モトローラ系のメモリ配置に対応。							*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		abp[]		: ASCIIﾋﾞｯﾄﾊﾟﾀｰﾝへのﾎﾟｲﾝﾀ							*/
/*		len			: ASCIIﾋﾞｯﾄﾊﾟﾀｰﾝのﾊﾞｲﾄ数(MAX= 8byte)				*/
/*																		*/
/* 戻り値																*/
/*		unsigned long	: ﾊﾞｲﾅﾘｰ･ﾋﾞｯﾄﾊﾟﾀｰﾝ								*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
union bp {
	unsigned long	lp;
	unsigned char	cp[sizeof(long)];
};

unsigned long
abp2bbp(
	unsigned char	*abp,
	int				len
	)
{
	union bp		bbp;
	unsigned char	*p= &bbp.cp[sizeof(long)-1];
	register int	i= 0;

	if((len == 0) || (len > (sizeof(long)*2)))		return(0);
	bbp.lp = 0;
	do {
		if(i & 0x1)  *(p--) |= (*abp & 0xf)<<4;
		else	     *p |= (*abp & 0xf);
		abp++;
	} while(++i < len);
	return((unsigned long)bbp.lp);
}
