/*----------------------------------------------------------------------*
 * File name	: scitx.c		/ sci tx								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/scitx.c 1.1 1996/01/19 14:45:18 kabumoto Exp $
 *$Log: scitx.c $
 * リビジョン 1.1  1996/01/19  14:45:18  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*		SCIに対して送信を行う。本モジュールはwrite関数のコーラー		*/
/*		側の関数となっている。											*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ										*/
/*		*u_buf		: string buffer										*/
/*		len			: number of byte									*/
/*																		*/
/* 戻り値																*/
/*		int			: 0	･･･ 正常終了									*/
/*----------------------------------------------------------------------*/

#include	<string.h>								/* ANSI std			*/
#include	"scictl.h"								/* Show current		*/


int
sci_tx(
	int				channel,					/* channel number		*/
	unsigned char	*u_buf,						/* string buffer pointer*/
	int				len							/* number of byte		*/
	)
{
	if(len)  return( sci_io_write(channel, u_buf, len));
	else	 return( sci_io_puts(channel, u_buf));
}

/*----------------------------------------------------------------------*/
/*		sci_io_write													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*		len			: number of byte									*/
/*																		*/
/* 戻り値																*/
/*		int			: n	･･･ 書き込んだﾊﾞｲﾄ数							*/
/*					 -1	･･･ system error								*/
/*					 -2	･･･ 空きｴﾘｱがない								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		ストリングスをリングバッファに書き込み、送信をイネーブル		*/
/*		にする。なお送信中でない場合の1STキャラクタの書き込みを			*/
/*		行う。															*/
/*----------------------------------------------------------------------*/

int
sci_io_write(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	unsigned char		c, *rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 送信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i, j;
	int					err, n;

	p = &sctx[ch];
	if(len == 0 || (n= len)> p->buf_max)				return(-1);
	i= j= p->putp;
	if((n +((i- p->getp)& p->buf_max)) >= p->buf_max)	return(-2);
	rp = p->buff;
	do {
		*(rp + i) = *(u_buf++);
		i = ++i & p->buf_max;
	} while(--n);
	p->putp = i;
	sci_io_tx(ch, p);
	return(len);
}

/*----------------------------------------------------------------------*/
/*		sci_io_puts														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*																		*/
/* 戻り値																*/
/*		int			: n	･･･ 書き込んだﾊﾞｲﾄ数							*/
/*					 -1	･･･ error										*/
/*					 -2	･･･ 空きｴﾘｱがない								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		ストリングスにスタート･フラグ/エンド･フラグ付加し、リング		*/
/*		バッファに書き込みを行った後、送信をイネーブルにする。			*/
/*		なお送信中でない場合の1STキャラクタの書き込みを行う。			*/
/*----------------------------------------------------------------------*/

int
sci_io_puts(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char		c, *rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 受信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	int					len;				/* number of byte			*/
	unsigned short		i, j;
	int					err, n;

	p = &sctx[ch];
	if((len = strlen((char *)u_buf))== 0 ||
	   (n= len) > p->buf_max-2)								return(-1);
	i= j= p->putp;
	if(((n+2)+((i- p->getp)& p->buf_max)) >= p->buf_max)	return(-2);
	rp = p->buff;
	*(rp + i) = LF,	i = ++i & p->buf_max;
	do {
		*(rp + i) = *(u_buf++);
		i = ++i & p->buf_max;
	} while(--n);
	*(rp + i) = CR,	 i = ++i & p->buf_max;
	p->putp = i;
	sci_io_tx(ch, p);
	return(len);
}
