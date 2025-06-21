/*----------------------------------------------------------------------*
 * File name	: scurx.c	/ scu rx				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scu/RCS/scurx.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scurx.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_scu/rcs/scurx.c 1.1 1993/10/20 14:34:50 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信リングバッファの内容を、指定バイト数または、テキスト単位	*/
/*	でユーザ･バッファにコピーする。scurx関数はread関数のコーラー	*/
/*	側となっている。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*u_buf		: string buffer					*/
/*	len 		: length					*/
/*									*/
/* 戻り値								*/
/*	int		: 0   ･･･ ｽﾄﾘﾝｸﾞ無し				*/
/*			  n   ･･･ character length			*/
/*			 -1   ･･･ error					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\rxdefd.h>		/* Show	drive		*/
#include	"scuctl.h"			/* Show current		*/

static const unsigned int	scu_rx_sem_id[SCU_CH_MAX] = {
								SCURX1_SEM_ID,
								SCURX2_SEM_ID};
static union aap		scu_rx_sem[SCU_CH_MAX];
static unsigned char		scu_rx_nest[SCU_CH_MAX] = {(unsigned char)0,
							   (unsigned char)0};

int scu_io_read(int, unsigned char *, int);
int scu_io_gets(int, unsigned char *);

int
scu_rx(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf,		/* string buffer pointer*/
	int			len		/* number of byte	*/
	)
{
	if(len)  return( scu_io_read( ch, u_buf, len));
	else	 return( scu_io_gets( ch, u_buf));
}

/*----------------------------------------------------------------------*/
/*	scu_io_read							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*u_buf  	: user buffer pointer				*/
/*	len 		: length					*/
/*									*/
/* 戻り値								*/
/*	int		: no of read byte				*/
/*									*/
/* 説明									*/
/*	受信リングバッファの内容を、指定バイト数(len)ユーザバッファ	*/
/*	にコピーしコピーしたバイト数を返す。指定バイト数のコピー終	*/
/*	了前に、バッファ内容をコピーし終えた場合(getp == putp)、そ	*/
/*	の時点で中断する。						*/
/*----------------------------------------------------------------------*/

int
scu_io_read(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf,		/* string buffer pointer*/
	int			len		/* length		*/
	)
{
	unsigned char		*rp;		/* ring bufferのﾎﾟｲﾝﾀｰ	*/
	int			i;
	int			err =0, n;

	if(scrx[ch].getp == scrx[ch].putp)			return(0);
	n = len;
	ADV_WAI_SEM(err, scu_rx_nest[ch], ENDLESS, scu_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)							return(-1);
	rp = scrx[ch].buff;
	i = scrx[ch].getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  scrx[ch].lcnt--;
		i = ++i & scrx[ch].buf_max;
		if(i == scrx[ch].putp)  break;
	} while(--n);
	scrx[ch].getp = (unsigned short)i;

	ADV_SIG_SEM(err, scu_rx_nest[ch], scu_rx_sem[ch].acadr);
	if(err)							return(-1);
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*	scu_io_gets							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*u_buf		: string buffer					*/
/*									*/
/* 戻り値								*/
/*	int		: 0   ･･･ ｽﾄﾘﾝｸﾞ無し				*/
/*			  n   ･･･ character length			*/
/*			 -1   ･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信リングバッファの内容を、ターミネータ(CR)単位でユーザバ	*/
/*	ッファにコピーしNULLを付加する。この関数内でスタート･フラグ	*/
/*	及びエンド･フラグ除去されるため、受信したフレームのバイト数	*/
/*	より-2された値が返される。					*/
/*----------------------------------------------------------------------*/

int
scu_io_gets(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf		/* string buffer pointer*/
	)
{
	unsigned char		*rp;		/* ring bufferのﾎﾟｲﾝﾀｰ	*/
	int			i;
	unsigned char		c;
	int			err =0, n =0, len;

	if(scrx[ch].lcnt == 0)					return(0);
	ADV_WAI_SEM(err, scu_rx_nest[ch], ENDLESS, scu_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)							return(-1);
	rp = scrx[ch].buff;
	i = scrx[ch].getp;
	do {
		c = *(rp + i);
		i = ++i & scrx[ch].buf_max;
		if(c == LF) {
			++n;
			continue;
		}
		else if(c == CR) {
			*u_buf = (unsigned char)'\0';
			len = ((i- scrx[ch].getp)& scrx[ch].buf_max) -(n+1);
			_disable();
			scrx[ch].getp = (unsigned short)i;
			scrx[ch].lcnt --;
			_enable();
			goto gse;
		}
		*(u_buf ++) = c;
	} while(i != scrx[ch].putp);
	_disable();
	scrx[ch].getp = (unsigned short)i;
	scrx[ch].lcnt = (unsigned short)0;
	_enable();
	len = -1;
gse:
	ADV_SIG_SEM(err, scu_rx_nest[ch], scu_rx_sem[ch].acadr);
	if(err)							return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_scu_rx							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信関係の処理の初期化を行う。					*/
/*----------------------------------------------------------------------*/

int
init_scu_rx(int ch)
{
	int		err;

	if(err = CRE_SEM( FIFO, &scu_rx_sem[ch].acadr,
			  scu_rx_sem_id[ch], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, scu_rx_sem[ch].acadr,
			  (unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
