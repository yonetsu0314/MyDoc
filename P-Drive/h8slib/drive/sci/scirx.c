/*----------------------------------------------------------------------*
 * File name	: scirx.c		/ sci rx								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/scirx.c 1.2 1996/01/20 10:52:32 kabumoto Exp $
 *$Log: scirx.c $
 * リビジョン 1.2  1996/01/20  10:52:32  kabumoto
 * 割り込み制御をCCRのIから、SCIのSCRのRIEに変更
 * 
 * リビジョン 1.1  1996/01/18  21:50:10  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                	  				*/
/*		受信リングバッファの内容を、指定バイト数または、テキスト単位	*/
/*		でユーザ･バッファにコピーする。scirx関数はread関数のコーラー	*/
/*		側となっている。												*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ										*/
/*		*u_buf		: string buffer										*/
/*		len 		: length											*/
/*																		*/
/* 戻り値																*/
/*		int			: 0   ･･･ ｽﾄﾘﾝｸﾞ無し								*/
/*					  n   ･･･ character length							*/
/*					 -1   ･･･ error										*/
/*----------------------------------------------------------------------*/

#include	<custom\custom.h>					/* show custom			*/
#include	"scictl.h"							/* Show current			*/
#include	"scictxt.h"							/*	"					*/


int
sci_rx(
	int				channel,					/* channel number		*/
	unsigned char	*u_buf,						/* string buffer pointer*/
	int				len							/* number of byte		*/
	)
{
	if(len)  return( sci_io_read(channel, u_buf, len));
	else	 return( sci_io_gets(channel, u_buf));
}

/*----------------------------------------------------------------------*/
/*		sci_io_read														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch			: channel no.										*/
/*		*u_buf  	: user buffer pointer								*/
/*		len 		: length											*/
/*																		*/
/* 戻り値																*/
/*		int			: no of read byte									*/
/*																		*/
/* 説明																	*/
/*		受信リングバッファの内容を、指定バイト数(len)ユーザバッファ		*/
/*		にコピーしコピーしたバイト数を返す。指定バイト数のコピー終		*/
/*		了前に、バッファ内容をコピーし終えた場合(getp == putp)、そ		*/
/*		の時点で中断する。												*/
/*----------------------------------------------------------------------*/

int
sci_io_read(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* length					*/
	)
{
	unsigned char		*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 受信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i;
	int					err, n;

	p = &scrx[ch];
	if(p->getp == p->putp)								return(0);
	n = len;
	rp = p->buff;
	i = p->getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  p->lcnt--;
		i = ++i & p->buf_max;
		if(i == p->putp)  break;
	} while(--n);
	p->getp = i;
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*		sci_io_gets														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*																		*/
/* 戻り値																*/
/*		int			: 0   ･･･ ｽﾄﾘﾝｸﾞ無し								*/
/*					  n   ･･･ character length							*/
/*					 -1   ･･･ error										*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		受信リングバッファの内容を、ターミネータ(CR)単位でユーザバ		*/
/*		ッファにコピーしNULLを付加する。この関数内でスタート･フラグ		*/
/*		及びエンド･フラグ除去されるため、受信したフレームのバイト数		*/
/*		より-2された値が返される。										*/
/*----------------------------------------------------------------------*/

int
sci_io_gets(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char		*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 受信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i;
	unsigned char		c, f;
	int					err, n =0, len;
	volatile struct st_sci*	cp;

	p = &scrx[ch];
	if(p->lcnt == 0)									return(0);
	rp = p->buff;
	i = p->getp;
	do {
		c = *(rp + i);
		i = ++i & p->buf_max;
		if(c == LF) {
			++n;
			continue;
		}
		else if(c == CR) {
			*u_buf = '\0';
			len = ((i- p->getp)& p->buf_max) -(n+1);
			p->getp = i;
			p->lcnt --;									return(len);
		}
		*(u_buf ++) = c;
	} while(i != p->putp);

	(cp = sci_ctxt_tbl[ch])->SCR.BIT.RIE = 0;
	p->getp = i;
	p->lcnt = 0;
	cp->SCR.BIT.RIE = 1;
	return(-1);
}
