/*----------------------------------------------------------------------*
 * File name	: abp2bbp.c	/ ascii ptn to binary ptn convert	*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/rcs/abp2bbp.c 1.3 1970/01/01 00:00:00 hiroe Exp $
 *$Log: abp2bbp.c $
 * リビジョン 1.3  1970/01/01  00:00:00  hiroe
 * 2009/04/01
 * ビッグエンディアンに対応する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	abpで示される指定バイト数のASCIIビットパターン(abp[0]のbit0	*/
/*	がLSB)をバイナリーのビットパターンに変換し32bitで結果を返す。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	abp[]		: ASCIIﾋﾞｯﾄﾊﾟﾀｰﾝへのﾎﾟｲﾝﾀ			*/
/*	len		: ASCIIﾋﾞｯﾄﾊﾟﾀｰﾝのﾊﾞｲﾄ数(MAX= 8byte)		*/
/*									*/
/* 戻り値								*/
/*	unsigned long	: ﾊﾞｲﾅﾘｰ･ﾋﾞｯﾄﾊﾟﾀｰﾝ				*/
/*----------------------------------------------------------------------*/

union bp {
	unsigned long	lp;
	unsigned char	cp[sizeof(long)];
};

unsigned long
abp2bbp(
	unsigned char	*abp,
	int		len
	)
{
	union bp	bbp;
	unsigned char	*p= &bbp.cp[0];
	int		i= 0;

	if((len == 0)||(len > sizeof(long)))		return((unsigned long)0L);
	bbp.lp = 0;
/*  --- リトルエンディアン
	do {
		if(i & 0x1)  *(p++) |= (unsigned char)((*abp & 0xf)<<4);
		else	     *p |= (unsigned char)(*abp & 0xf);
		abp++;
	} while(++i < len);
*/
/*  --- ビッグエンディアン */
	abp += (len-1);
	do {
		if(i & 0x1)  *(p++) |= (unsigned char)(*abp & 0xf);
		else	     *p |= (unsigned char)((*abp & 0xf)<<4);
		abp--;
	} while(++i < len);
	return(bbp.lp);
}
