/*----------------------------------------------------------------------*
 * File name	: scurx.c	/ scu rx									*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scurx.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scurx.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/26　誤記修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scurx.c 1.1 1970/01/01
 *Log: scurx.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.				*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	受信リングバッファの内容を、指定バイト数または、テキスト単位		*/
/*	でユーザ･バッファにコピーする。scurx関数はread関数のコーラー		*/
/*	側となっている。													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ											*/
/*	*u_buf		: string buffer											*/
/*	len 		: length												*/
/*																		*/
/* 戻り値																*/
/*	int		: 0   ･･･ ｽﾄﾘﾝｸﾞ無し										*/
/*			  n   ･･･ character length									*/
/*			 -1   ･･･ error												*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>						/* Show xlib			*/
#include	<custom\custom.h>					/* Show custom			*/
#include	<drive\rxdefd.h>					/* Show	drive			*/
#include	"scuctl.h"							/* Show current			*/

static const unsigned int	m66230_rx_sem_id[M66230_CH_MAX] = {
															MPS0RX_SEM_ID,
															MPS0RX_SEM_ID};
static union aap		m66230_rx_sem[M66230_CH_MAX];
static unsigned char	m66230_rx_nest[M66230_CH_MAX] = {	0,
															0};

int m66230_io_read(int, unsigned char *, int);
int m66230_io_gets(int, unsigned char *);

int
m66230_rx(
	int				ch,					/* channel number			*/
	unsigned char	*u_buf,				/* string buffer pointer	*/
	int				len					/* number of byte			*/
	)
{
	if(len)  return( m66230_io_read( ch, u_buf, len));
	else	 return( m66230_io_gets( ch, u_buf));
}

/*----------------------------------------------------------------------*/
/*	m66230_io_read														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	*u_buf  	: user buffer pointer									*/
/*	len 		: length												*/
/*																		*/
/* 戻り値																*/
/*	int		: no of read byte											*/
/*																		*/
/* 説明																	*/
/*	受信リングバッファの内容を、指定バイト数(len)ユーザバッファ			*/
/*	にコピーしコピーしたバイト数を返す。指定バイト数のコピー終			*/
/*	了前に、バッファ内容をコピーし終えた場合(getp == putp)、そ			*/
/*	の時点で中断する。													*/
/*----------------------------------------------------------------------*/

int
m66230_io_read(
	int				ch,					/* channel number			*/
	unsigned char	*u_buf,				/* string buffer pointer	*/
	int				len					/* length					*/
	)
{
	unsigned char	*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	int				i;
	int				err =0, n;

	if(m66230rx[ch].getp == m66230rx[ch].putp)			return(0);
	n = len;
	ADV_WAI_SEM(err, m66230_rx_nest[ch], ENDLESS, m66230_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)												return(-1);
	rp = m66230rx[ch].buff;
	i = m66230rx[ch].getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  m66230rx[ch].lcnt--;
		i = ++i & m66230rx[ch].buf_max;
		if(i == m66230rx[ch].putp)  					break;
	} while(--n);
	m66230rx[ch].getp = (unsigned short)i;

	ADV_SIG_SEM(err, m66230_rx_nest[ch], m66230_rx_sem[ch].acadr);
	if(err)												return(-1);
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*	m66230_io_gets														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	*u_buf		: string buffer											*/
/*																		*/
/* 戻り値																*/
/*	int		: 0   ･･･ ｽﾄﾘﾝｸﾞ無し										*/
/*			  n   ･･･ character length									*/
/*			 -1   ･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	受信リングバッファの内容を、ターミネータ(CR)単位でユーザバ			*/
/*	ッファにコピーしNULLを付加する。この関数内でスタート･フラグ			*/
/*	及びエンド･フラグ除去されるため、受信したフレームのバイト数			*/
/*	より-2された値が返される。											*/
/*----------------------------------------------------------------------*/

int
m66230_io_gets(
	int				ch,						/* channel number			*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char	*rp;					/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	int				i;
	unsigned char	c;
	int				err =0, n =0, len;

	if(m66230rx[ch].lcnt == 0)						return(0);
	ADV_WAI_SEM(err, m66230_rx_nest[ch], ENDLESS, m66230_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)											return(-1);
	rp = m66230rx[ch].buff;
	i = m66230rx[ch].getp;
	do {
		c = *(rp + i);
		i = ++i & m66230rx[ch].buf_max;
		if(c == LF) {
			++n;
			continue;
		}
		else if(c == CR) {
			*u_buf = (unsigned char)'\0';
			len = ((i- m66230rx[ch].getp)& m66230rx[ch].buf_max) -(n+1);
			_disable();
			m66230rx[ch].getp = (unsigned short)i;
			m66230rx[ch].lcnt --;
			_enable();
			goto gse;
		}
		*(u_buf ++) = c;
	} while(i != m66230rx[ch].putp);
	_disable();
	m66230rx[ch].getp = (unsigned short)i;
	m66230rx[ch].lcnt = (unsigned short)0;
	_enable();
	len = -1;
gse:
	ADV_SIG_SEM(err, m66230_rx_nest[ch], m66230_rx_sem[ch].acadr);
	if(err)											return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_m66230_rx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0	･･･ 成功													*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	受信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_m66230_rx(int ch)
{
	int		err;

	if(err = CRE_SEM( FIFO, &m66230_rx_sem[ch].acadr,
			  m66230_rx_sem_id[ch], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, m66230_rx_sem[ch].acadr,
			  (unsigned int)1, VOID_TIME))				return(-1);
	return(0);
}
