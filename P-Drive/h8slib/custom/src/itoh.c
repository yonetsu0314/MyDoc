/*----------------------------------------------------------------------*
 * File name	: itoh.c		/ convert to ascii hex					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/custom/src/rcs/itoh.c 1.2 1996/04/04 19:20:27 minaguchi Exp $
 *$Log: itoh.c $
 * リビジョン 1.2  1996/04/04  19:20:27  minaguchi
 * ltoh関数がインテル系のメモリ配置対応であったのをモトローラ系のメモリ配置
 * 対応に変更した。
 * 
 * リビジョン 1.1  1996/01/19  10:51:49  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
static char
get_high(
	char	data
	)
{
	char	data1;
	data1 = data & 240;
	data1 = data1 >>4;
	data1 = data1 & 15;
	return(data1);
}

static char
get_low(
	char	data
	)
{
	char	data1;
	return(data & 15);
}

static char
trans_ascii(
	char	data
	)
{
	if(data <=9)	return(data+48);
	else			return(data+55);
}

/*----------------------------------------------------------------------*/
/*		itoh															*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*		intを4byteのHEX ASCIIに変換する。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		i		: int value												*/
/*		*s		: pointer to string										*/
/*																		*/
/* 戻り値																*/
/*		void	: 無し													*/
/*----------------------------------------------------------------------*/

itoh(i,s)
	register int	i;								/* converted value	*/
	register char	*s;
{
	s[0] = trans_ascii(get_high((char)((i >> 8) & 0xff)));
	s[1] = trans_ascii(get_low((char)((i >> 8) & 0xff)));
	s[2] = trans_ascii(get_high((char)(i & 0xff)));
	s[3] = trans_ascii(get_low((char)(i & 0xff)));
}

/*----------------------------------------------------------------------*/
/*		ctoh															*/
/*----------------------------------------------------------------------*/
/* 説明                                         		         		*/
/*		charを2byteのHEX ASCIIに変換する。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		i		: int value												*/
/*		*s		: pointer to string										*/
/*																		*/
/* 戻り値																*/
/*		void	: 無し													*/
/*----------------------------------------------------------------------*/

ctoh(
	unsigned char	i,
	register char	*s
	)
{
	s[0] = trans_ascii(get_high((char)(i & 0xff)));
	s[1] = trans_ascii(get_low((char)(i & 0xff)));
}

/*----------------------------------------------------------------------*/
/*		ltoh															*/
/*----------------------------------------------------------------------*/
/* 説明                                     		             		*/
/*		longを8byteのHEX ASCIIに変換する。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		l		: long value											*/
/*		*s		: pointer to string										*/
/*																		*/
/* 戻り値																*/
/*		void	: 無し													*/
/*----------------------------------------------------------------------*/

ltoh(
	long		l,
	char		*s
	)
{
	unsigned char	c, *p= (unsigned char *)&l, n= sizeof(l);

	do {
		c = *(p++);
		*(s++) = trans_ascii(get_high(c));
		*(s++) = trans_ascii(get_low(c));
	} while(--n);
}
