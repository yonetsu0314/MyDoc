/*----------------------------------------------------------------------*
 * File name	: scutx.c	/ scu tx									*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scutx.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scutx.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scutx.c 1.1 1970/01/01
 *Log: scutx.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.				*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	SCUに対して送信を行う。本モジュールはwrite関数のコーラー			*/
/*	側の関数となっている。												*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ											*/
/*	*u_buf		: string buffer											*/
/*	len		: number of byte											*/
/*																		*/
/* 戻り値																*/
/*	int		: 0	･･･ 正常終了											*/
/*----------------------------------------------------------------------*/

#include	<string.h>							/* ANSI std				*/
#include	<rxif\rx116.h>						/* Show xlib			*/
#include	<custom\custom.h>					/* Show custom			*/
#include	<drive\rxdefd.h>					/* Show	drive			*/
#include	"scuctl.h"							/* Show current			*/

static const unsigned int	m66230_tx_sem_id[M66230_CH_MAX] = { MPS0TX_SEM_ID,
															 	MPS1TX_SEM_ID};
static union aap			m66230_tx_sem[M66230_CH_MAX];
static unsigned char		m66230_tx_nest[M66230_CH_MAX] = {0, 0};
int m66230_io_write(int, unsigned char *, int);
int m66230_io_puts(int, unsigned char *);

int
m66230_tx(
	int				ch,					/* channel number			*/
	unsigned char	*u_buf,				/* string buffer pointer	*/
	int				len					/* number of byte			*/
	)
{
	if(len)  return( m66230_io_write( ch, u_buf, len));
	else	 return( m66230_io_puts( ch, u_buf));
}

/*----------------------------------------------------------------------*/
/*	m66230_io_write														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	*u_buf		: string buffer											*/
/*	len		: number of byte											*/
/*																		*/
/* 戻り値																*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数									*/
/*			 -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	ストリングスをリングバッファに書き込み、送信をイネーブル			*/
/*	にする。なお送信中でない場合の1STキャラクタの書き込み、及			*/
/*	びリングバッファ･フルの状態下では、回線を獲得後直接SCUに			*/
/*	データの書き込みを行う。											*/
/*----------------------------------------------------------------------*/

int
m66230_io_write(
	int				ch,						/* channel number			*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	unsigned char	*rp;					/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	int				i;
	int				err =0, n;

	if(len == 0 || len > m66230tx[ch].buf_max)			return(-1);
	ADV_WAI_SEM(err, m66230_tx_nest[ch], ENDLESS, m66230_tx_sem[ch].acadr,
		    VOID_TIME);
	if(err)							return(-1);
	do {
		n = len;
		if((n +((m66230tx[ch].putp- m66230tx[ch].getp)& m66230tx[ch].buf_max)) < m66230tx[ch].buf_max) {
			rp = m66230tx[ch].buff;
			i = m66230tx[ch].putp;
			do {
				*(rp + i) = *(u_buf++);
				i = ++i & m66230tx[ch].buf_max;
			} while(--n);
			m66230tx[ch].putp = (unsigned short)i;
		}
	} while(m66230_io_tx( ch, &m66230tx[ch], n));

	ADV_SIG_SEM(err, m66230_tx_nest[ch], m66230_tx_sem[ch].acadr);
	if(err)							return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	m66230_io_puts														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	*u_buf		: string buffer											*/
/*																		*/
/* 戻り値																*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数									*/
/*			 -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	ストリングスにスタート･フラグ/エンド･フラグ付加し、リング			*/
/*	バッファに書き込みを行った後、送信をイネーブルにする。				*/
/*	なお送信中でない場合の1STキャラクタの書き込み、及びリング			*/
/*	バッファ･フルの状態下では、回線を獲得後直接scuにデータの			*/
/*	書き込みを行う。													*/
/*----------------------------------------------------------------------*/

int
m66230_io_puts(
	int				ch,						/* channel number			*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char	*rp;					/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	int				len;					/* number of byte			*/
	int				i;
	int				err =0, n;

	if((len = strlen( (char *)u_buf))== 0 
					|| len > m66230tx[ch].buf_max-2)		return(-1);
	ADV_WAI_SEM(err, m66230_tx_nest[ch], ENDLESS, m66230_tx_sem[ch].acadr,
		    VOID_TIME);
	if(err)													return(-1);
	do {
		n = len +2;
		if((n +((m66230tx[ch].putp- m66230tx[ch].getp)& m66230tx[ch].buf_max)) < m66230tx[ch].buf_max) {
			n -= 2;
			rp = m66230tx[ch].buff;
			i = m66230tx[ch].putp;
			*(rp + i) = (unsigned char)LF,	i = ++i & m66230tx[ch].buf_max;
			do {
				*(rp + i) = *(u_buf++);
				i = ++i & m66230tx[ch].buf_max;
			} while(--n);
			*(rp + i) = (unsigned char)CR,	i = ++i & m66230tx[ch].buf_max;
			m66230tx[ch].putp = (unsigned short)i;
		}
	} while( m66230_io_tx( ch, &m66230tx[ch], n));

	ADV_SIG_SEM(err, m66230_tx_nest[ch], m66230_tx_sem[ch].acadr);
	if(err)													return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_m66230_tx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0	･･･ 成功													*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	送信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_m66230_tx(int ch)
{
	int		err;

	if(err = CRE_SEM( FIFO, &m66230_tx_sem[ch].acadr,
					m66230_tx_sem_id[ch], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, m66230_tx_sem[ch].acadr,
					(unsigned int)1, VOID_TIME))			return(-1);
	return(0);
}
