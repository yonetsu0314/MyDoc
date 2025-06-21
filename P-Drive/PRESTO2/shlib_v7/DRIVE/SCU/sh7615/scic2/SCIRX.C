/*----------------------------------------------------------------------*
 * File name	: scurx.c	/ scu rx									*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scirx.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic2/RCS/scirx.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scirx.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/24 biosif経由とする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA SCALES MFG. CO., LTD.				*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
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
#include	<kernel.h>						/* Show current				*/
#include	<rxif\rx116.h>					/* Show xlib				*/
#include	<drive\rxdefd.h>				/* Show	drive				*/
#include	"scictl.h"						/* Show current				*/

int	scu_io_read(int,unsigned char *,int);
int	scu_io_gets(int,unsigned char *);

static unsigned int		scif_rx_sem_id[SCIF_CH_MAX] = {
						SCIF1RX_SEM_ID, SCIF2RX_SEM_ID
};
static union aap		scif_rx_sem[SCIF_CH_MAX] = {
						0L					/* RXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ		*/
};
static unsigned char	scif_rx_nest[SCIF_CH_MAX] = {
						0					/* SCIF関数呼出のﾈｽﾃｨﾝｸﾞ回数*/
};

int
scif_rx(
	int				channel,				/* CON channel number		*/
	unsigned char 	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	if(len)  return( scif_io_read(channel, u_buf, len) );
	else	 return( scif_io_gets(channel, u_buf) );
}

/*----------------------------------------------------------------------*/
/*	scif_io_read														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	ch			: channel no.											*/
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
scif_io_read(
	int				ch,						/* channel no.				*/
	unsigned char 	*u_buf,					/* string buffer pointer	*/
	int				len						/* length					*/
	)
{
	unsigned char		*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 送信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i;
	int					err =0;
	int					n;

	p = &scrx[ch];
	if(p->getp == p->putp)		return(0);
	n = len;
	ADV_WAI_SEM(err, scif_rx_nest[ch], ENDLESS, scif_rx_sem[ch].acadr, VOID_TIME);
	if(err)						return(-1);
	rp = p->buff;
	i = p->getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  p->lcnt--;
		i = ++i & p->buf_max;
		n--;	// new
		if(i == p->putp)  break;
//	} while(--n);
	} while(n);	//new
	p->getp = i;

	ADV_SIG_SEM(err, scif_rx_nest[ch], scif_rx_sem[ch].acadr);
	if(err)						return(-1);
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*	scif_io_gets														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	ch			: channel no.											*/
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
scif_io_gets(
	int				ch,						/* channel no.				*/
	unsigned char 	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char		*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 受信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i;
	unsigned char		c;
	int					err =0, n =0, len;

	p = &scrx[ch];
	if(p->lcnt == 0)				return(0);
	ADV_WAI_SEM(err, scif_rx_nest[ch], ENDLESS, scif_rx_sem[ch].acadr, VOID_TIME);
	if(err)							return(-1);
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
			_disable();
			p->getp = i;
			p->lcnt --;
			_enable();
			goto gse;
		}
		*(u_buf ++) = c;
	} while(i != p->putp);
	_disable();
	p->getp = i;
	p->lcnt = 0;
	_enable();
	len = -1;
gse:
	ADV_SIG_SEM(err, scif_rx_nest[ch], scif_rx_sem[ch].acadr);
	if(err)							return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_scif1_rx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0		･･･ 成功												*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*	受信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_scif1_rx(void)
{
	int		err;

	if(err = CRE_SEM( FIFO, &scif_rx_sem[SCIF_CH1].acadr, scif_rx_sem_id[SCIF_CH1], 1))	return(-1);
	if(err = WAI_SEM( ENDLESS, scif_rx_sem[SCIF_CH1].acadr,1, VOID_TIME))				return(-1);
	return(0);
}

/*----------------------------------------------------------------------*/
/*	init_scif2_rx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0		･･･ 成功												*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*	受信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_scif2_rx(void)
{
	int		err;

	if(err = CRE_SEM( FIFO, &scif_rx_sem[SCIF_CH2].acadr, scif_rx_sem_id[SCIF_CH2], 1))	return(-1);
	if(err = WAI_SEM( ENDLESS, scif_rx_sem[SCIF_CH2].acadr,1, VOID_TIME))				return(-1);
	return(0);
}
