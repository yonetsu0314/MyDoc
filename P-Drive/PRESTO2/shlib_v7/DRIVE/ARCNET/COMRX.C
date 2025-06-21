/*----------------------------------------------------------------------*
 * File name	: comrx.c	/ arcnet rx				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcnet/RCS/comrx.c 1.6 1970/01/01 00:00:00 chimura Exp $
 *$Log: comrx.c $
 * リビジョン 1.6  1970/01/01  00:00:00  chimura
 * 2003/08/13 前リビジョンの不具合修正
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/07/29 9:00
 * 関数arcnet_io_read内で使用しているオート変数nの型をintからshortに変更する。
 * この変数は、２バイトのテキスト長を表現するために使用しており、int型にする
 * とテキスト長があわなくなる。
 * また、テキストの最後に'\0'がないテキストを受信すると実際のテキスト長より
 * 長いデータを送信する。'\0'がなくても正しくテキストを送信するように修正
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  debug-5-1
 * 2003/03/13  14:45
 * arcnet_io_gets() にてint型ローカル変数をshort型に変更した。
 * （SHではint型4バイトのため、sizeof(n)の値が変わる。
 *   このため、実際とは2バイトずれたところをチェックサムとみなしてしまい、
 *   うまく動作しなかった。）
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/02/06 13:30
 * arcnet_io_gets()でrtpをintでキャストしている不具合修正。
 * レングスのため、unsigned short でキャストしなければならない。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comrx.c 1.1 1995/11/07 17:12:43 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"comctl.h"			/* Show current		*/

/*----------------------------------------------------------------------*/
/*	arcnet_io_read							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	受信バッファの内容を、指定バイト数(len)ユーザバッファに		*/
/*	コピーし、転送バイト数を返す。指定バイト数のコピー終了前	*/
/*	にテキストをコピーし終えた場合、その時点で完了する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno 	: channel number					*/
/*	*u_buf  : pointer to user buffer				*/
/*	len 	: length						*/
/*									*/
/* 戻り値								*/
/*	int	: +N	･･･ no of read byte				*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

int
arcnet_io_read(
	int			devno,
	unsigned char		*u_buf,
	int			len
	)
{
/*	struct rxh	*ph;*/			/* pointer to rx header	*/
	unsigned char	tmp[LPKT_MAX], *rtp =&tmp[0];
	unsigned short	n;

	if(arc_dpm_read(devno, &rtp) < 0)			return(-1);
	if(rtp == NULL)						return(0);
	memcpy((void *)u_buf, (void *)(rtp+sizeof(short)), (size_t)(n= *((unsigned short *)rtp)));
	if(rtp != tmp)  free((void *)rtp);
	u_buf[(unsigned int)n] = (unsigned char)'\0';	/*修正*/
	return((int)n);
}

/*----------------------------------------------------------------------*/
/*	arcnet_io_gets							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信リングバッファの内容を、ターミネータ(CR)単位でユーザバ	*/
/*	ッファにコピーしNULLを付加する。この関数内でスタート･フラグ	*/
/*	及びエンド･フラグ除去されるため、受信したフレームのバイト数	*/
/*	より-2された値が返される。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ						*/
/*	*u_buf  : pointer to user buffer				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ ｽﾄﾘﾝｸﾞ無し					*/
/*		  +N	･･･ character length				*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

int
arcnet_io_gets(
	int			devno,
	unsigned char		*u_buf
	)
{
/*	struct rxh	*ph;*/			/* pointer to rx header	*/
	unsigned char	tmp[LPKT_MAX], *rtp =tmp, *p;
	short		n;
	int		i;

	if(arc_dpm_read(devno, &rtp) < 0)			return(-1);
	if(rtp == NULL)						return(0);
	n = *((unsigned short *)rtp);
	p = rtp+sizeof(n);
	*(u_buf++) = *(p++);
	*(u_buf++) = *(p++);
	for(i =n-2 ; i ; i--, p++) {
		if(*p == LF)
			n--;
		else if(*p == CR) {
		   	n--;
			*u_buf = (unsigned char)'\0';
		}
		else
			*(u_buf++) = *p;
	}
	if(rtp != tmp)  free((void *)rtp);
	return((int)n);
}

int
arcnet_rx(
	int		devno,
	unsigned char	*u_buf,
	int		len
	)
{
	if(len)  return( arcnet_io_read( devno, u_buf, len));
	else	 return( arcnet_io_gets( devno, u_buf));
}

