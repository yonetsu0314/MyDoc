/*----------------------------------------------------------------------*
 * File name	: fifrx.c	/ fifo rx				*
 * Original	: p:/presto/shlib/drive/scu/RCS/scurx.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoscuc/rcs/fifrx.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifrx.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 04/12/07 MISCﾗｲﾌﾞﾗﾘからBIOSｺｰﾙに変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
 * 
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
#include	<biosif\fifo.h>		/*	"		*/
#include	"fifctl.h"			/* Show current		*/
#include	"fpgafifo.h"

static const unsigned int	fif_rx_sem_id[FIF_CH_MAX] = {
								FIFRX1_SEM_ID,
								FIFRX2_SEM_ID};
static union aap		fif_rx_sem[FIF_CH_MAX];
static unsigned char		fif_rx_nest[FIF_CH_MAX] = {(unsigned char)0,
							   (unsigned char)0};

int fif_io_read(int, unsigned char *, int);
int fif_io_gets(int, unsigned char *);

int
fifo_rx(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf,		/* string buffer pointer*/
	int			len		/* number of byte	*/
	)
{
	if(len)  return( fif_io_read( ch, u_buf, len));
	else	 return( fif_io_gets( ch, u_buf));
}

/*----------------------------------------------------------------------*/
/*	fif_io_read							*/
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
fif_io_read(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf,		/* string buffer pointer*/
	int			len		/* length		*/
	)
{
	unsigned char		*rp;		/* ring bufferのﾎﾟｲﾝﾀｰ	*/
	int			i;
	int			err =0, n;

	if(fifrx[ch].getp == fifrx[ch].putp)			return(0);
	n = len;
	ADV_WAI_SEM(err, fif_rx_nest[ch], ENDLESS, fif_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)							return(-1);
	rp = fifrx[ch].buff;
	i = fifrx[ch].getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  fifrx[ch].lcnt--;
		i = ++i & fifrx[ch].buf_max;
		n--;	// new
		if(i == fifrx[ch].putp)  break;
	} while(n);	//new
	fifrx[ch].getp = (unsigned short)i;
	if(fifrx[ch].flag & RXPEND){
		_disable();
		fifrx[ch].flag &= ~RXPEND;
		fpgamskflg &= ~FPGA_RX_MSK;
		_fifo_msktxrx(fpgamskflg);
		_enable();
	}

	ADV_SIG_SEM(err, fif_rx_nest[ch], fif_rx_sem[ch].acadr);
	if(err)							return(-1);
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*	fif_io_gets							*/
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
fif_io_gets(
	int			ch,		/* channel number	*/
	unsigned char		*u_buf		/* string buffer pointer*/
	)
{
	unsigned char		*rp;		/* ring bufferのﾎﾟｲﾝﾀｰ	*/
	int			i;
	unsigned char		c;
	int			err =0, n =0, len;

	if(fifrx[ch].lcnt == 0)					return(0);
	ADV_WAI_SEM(err, fif_rx_nest[ch], ENDLESS, fif_rx_sem[ch].acadr,
		    VOID_TIME);
	if(err)							return(-1);
	rp = fifrx[ch].buff;
	i = fifrx[ch].getp;
	do {
		c = *(rp + i);
		i = ++i & fifrx[ch].buf_max;
		if(c == LF) {
			++n;
			continue;
		}
		else if(c == CR) {
			*u_buf = (unsigned char)'\0';
			len = ((i- fifrx[ch].getp)& fifrx[ch].buf_max) -(n+1);
			_disable();
			fifrx[ch].getp = (unsigned short)i;
			fifrx[ch].lcnt --;
			_enable();
			goto gse;
		}
		*(u_buf ++) = c;
	} while(i != fifrx[ch].putp);
	_disable();
	fifrx[ch].getp = (unsigned short)i;
	fifrx[ch].lcnt = (unsigned short)0;
	_enable();
	len = -1;
gse:
	if(fifrx[ch].flag & RXPEND){
		_disable();
		fifrx[ch].flag &= ~RXPEND;
		fpgamskflg &= ~FPGA_RX_MSK;
		_fifo_msktxrx(fpgamskflg);
		_enable();
	}

	ADV_SIG_SEM(err, fif_rx_nest[ch], fif_rx_sem[ch].acadr);
	if(err)							return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_fif_rx							*/
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
init_fif_rx(int ch)
{
	int		err;

	if(err = CRE_SEM( FIFO, &fif_rx_sem[ch].acadr,
			  fif_rx_sem_id[ch], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, fif_rx_sem[ch].acadr,
			  (unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
