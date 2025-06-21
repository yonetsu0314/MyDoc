/*----------------------------------------------------------------------*
 * File name	: bbp2abp.c		/ binary ptn to ascii ptn convert		*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/custom/src/RCS/bbp2abp.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: bbp2abp.c $
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
/*		バイナリーのビットパターンbbdをLSBより4bitニブル単位で			*/
/*		ASCII(0x30:'0'-0x3f:'?')に変換し、abp[]に指定バイト数スト		*/
/*		アーする。														*/
/*																		*/
/*      注） モトローラ系のメモリ配置に対応。							*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		abp[]	: ASCIIﾋﾞｯﾄﾊﾟﾀｰﾝ出力先へのﾎﾟｲﾝﾀ							*/
/*		bbd		: 入力するﾊﾞｲﾅﾘｰ･ﾋﾞｯﾄﾊﾟﾀｰﾝ								*/
/*		len		: 変換するASCIIﾋﾞｯﾄﾊﾟﾀｰﾝのﾊﾞｲﾄ数						*/
/*																		*/
/* 戻り値																*/
/*		int		: 0		･･･ successful complete							*/
/*				  -1	･･･ error code									*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
int
bbp2abp(
	unsigned char	*abp,
	unsigned long	bbd,
	int				len
	)
{
	register int	i= 0;
	unsigned char	 *bbp;

	if( len == 0)								return(-1);
	if( len > (sizeof(long)*2))					return(-1);

	bbp = (unsigned char *)&bbd;
	bbp += sizeof(long);
	
	do {
		if(i & 0x1)  *abp = ((*(bbp--) & 0xf0)>>4)|'0';
		else	     *abp = ((*bbp & 0x0f))|'0';
		abp++;
	} while(++i < len);
	return(0);
}
