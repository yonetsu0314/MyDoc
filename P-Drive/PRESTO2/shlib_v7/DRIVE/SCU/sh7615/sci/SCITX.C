/*----------------------------------------------------------------------*
 * File name	: scutx.c	/ scu tx									*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scitx.c 1.2
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/sci/RCS/scitx.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scitx.c $
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

#include	<string.h>						/* ANSI std					*/
#include	<rxif\rx116.h>					/* Show xlib				*/
#include	<drive\rxdefd.h>				/* Show	drive				*/
#include	"scictl.h"						/* Show current				*/

static unsigned int		scif_tx_sem_id[SCIF_CH_MAX] = {
						SCIF1TX_SEM_ID, SCIF2TX_SEM_ID
};
static union aap		scif_tx_sem[SCIF_CH_MAX] = {
						0L					/* TXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ		*/
};
static unsigned char	scif_tx_nest[SCIF_CH_MAX] = {
						0					/* SCIF関数呼出のﾈｽﾃｨﾝｸﾞ回数*/
};

int	sci_io_write(int,unsigned char *,int);
int	sci_io_puts(int,unsigned char *);

int
scif_tx(
	int				channel,				/* channel number			*/
	unsigned char 	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	if(len)  return( sci_io_write( channel, u_buf, len));
	else	 return( sci_io_puts( channel, u_buf));
}

/*----------------------------------------------------------------------*/
/*	sci_io_write														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	ch			: channel no.											*/
/*	*u_buf		: string buffer											*/
/*	len			: number of byte										*/
/*																		*/
/* 戻り値																*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数									*/
/*			 -1	･･･ 送信error											*/
/*			 -2	･･･ 空きｴﾘｱがない										*/
/*			 -3	･･･ ｾﾏﾌｫｴﾗｰ												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	ストリングスをリングバッファに書き込み、送信をイネーブル			*/
/*	にする。なお送信中でない場合の1STキャラクタの書き込み、及			*/
/*	びリングバッファ･フルの状態下では、回線を獲得後直接SCUに			*/
/*	データの書き込みを行う。											*/
/*----------------------------------------------------------------------*/

int
sci_io_write(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	unsigned char		*rp;				/* ring bufferのﾎﾟｲﾝﾀｰ		*/
	struct comh			*p;					/* 送信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ	*/
	unsigned short		i;
	int					err = 0;
	int					n;

	p = &sctx[ch];
	if(len == 0 || len > p->buf_max)			return(-1);
	ADV_WAI_SEM(err, scif_tx_nest[ch], ENDLESS, scif_tx_sem[ch].acadr, VOID_TIME);
	if(err)										return(-3);
	do {
		n = len;
		if((n +((p->putp-p->getp)& p->buf_max)) < p->buf_max) {
			rp = p->buff;
			i = p->putp;
			do {
				*(rp + i) = *(u_buf++);
				i = (unsigned short)( ++i & p->buf_max);
			} while(--n);
			p->putp = i;
		} else {
			len = -2;
			goto eee;
		}
	} while(scif_io_tx(ch, p, n));
eee:
	ADV_SIG_SEM(err, scif_tx_nest[ch], scif_tx_sem[ch].acadr);
	if(err)										return(-3);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	sci_io_puts															*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch		: channel no.											*/
/*	*u_buf		: string buffer											*/
/*																		*/
/* 戻り値																*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数									*/
/*			 -1	･･･ 送信error											*/
/*			 -2	･･･ 空きｴﾘｱがない										*/
/*			 -3	･･･ ｾﾏﾌｫｴﾗｰ												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	ストリングスにスタート･フラグ/エンド･フラグ付加し、リング			*/
/*	バッファに書き込みを行った後、送信をイネーブルにする。				*/
/*	なお送信中でない場合の1STキャラクタの書き込み、及びリング			*/
/*	バッファ･フルの状態下では、回線を獲得後直接scuにデータの			*/
/*	書き込みを行う。													*/
/*----------------------------------------------------------------------*/

int
sci_io_puts(
	int				ch,							/* channel no.			*/
	unsigned char 	*u_buf						/* string buffer pointer*/
	)
{
	unsigned char		*rp;					/* ring bufferのﾎﾟｲﾝﾀｰ	*/
	struct comh			*p;						/* 受信管理ﾍｯﾀﾞへのﾎﾟｲﾝﾀ*/
	int					len;					/* number of byte		*/
	unsigned short		i;
	int					err =0;
	int					n;

	p = &sctx[ch];
	if((len = (int)strlen((char *)u_buf))== 0 || len > p->buf_max-2)	return(-1);
	ADV_WAI_SEM(err, scif_tx_nest[ch], ENDLESS, scif_tx_sem[ch].acadr, VOID_TIME);
	if(err)		return(-3);
	do {
		n = len +2;
		if((n +((p->putp- p->getp)& p->buf_max)) < p->buf_max) {
			n -= 2;
			rp = p->buff;
			i = p->putp;
			*(rp + i) = LF,	i = ++i & p->buf_max;
			do {
				*(rp + i) = *(u_buf++);
				i = ++i & p->buf_max;
			} while(--n);
			*(rp + i) = CR,	i = ++i & p->buf_max;
			p->putp = i;
		}
		else{
			len = -2;
			goto eee;
		}
	} while( scif_io_tx( ch, p, n));
eee:
	ADV_SIG_SEM(err, scif_tx_nest[ch], scif_tx_sem[ch].acadr);
	if(err)		return(-3);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_scif1_tx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0		･･･ 成功												*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*	送信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_scif1_tx(void)
{
	int		err;

	if(err = CRE_SEM( FIFO, &scif_tx_sem[SCIF_CH1].acadr, scif_tx_sem_id[SCIF_CH1], 1))	return(-1);
	if(err = WAI_SEM( ENDLESS, scif_tx_sem[SCIF_CH1].acadr, 1, VOID_TIME))				return(-1);
	return(0);
}

/*----------------------------------------------------------------------*/
/*	init_scif2_tx														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	void	: 無し														*/
/*																		*/
/* 戻り値																*/
/*	int	: 0		･･･ 成功												*/
/*		  -1	･･･ error												*/
/*----------------------------------------------------------------------*/
/* 説明                                                 		 		*/
/*	送信関係の処理の初期化を行う。										*/
/*----------------------------------------------------------------------*/

int
init_scif2_tx(void)
{
	int		err;

	if(err = CRE_SEM( FIFO, &scif_tx_sem[SCIF_CH2].acadr, scif_tx_sem_id[SCIF_CH2], 1))	return(-1);
	if(err = WAI_SEM( ENDLESS, scif_tx_sem[SCIF_CH2].acadr, 1, VOID_TIME))				return(-1);
	return(0);
}
