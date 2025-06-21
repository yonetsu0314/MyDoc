/*----------------------------------------------------------------------*
 * File name	: scu1int.c	/ scu interrupt handler			*
 * Original	: p:/presto/shlib/drive/scu/RCS/scu1int.c 1.3
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoarc/rcs/fifint.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifint.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 04/12/07 MISCﾗｲﾌﾞﾗﾘからBIOSｺｰﾙに変更
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * デバッグ用fif_int_loop追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* Function								*/
/*	FIFOの割り込みﾊﾝﾄﾞﾗﾓｼﾞｭｰﾙ。					*/
/*----------------------------------------------------------------------*/
#include <kernel.h>
#include <machine.h>
#include	<biosif\fifo.h>		/*	"		*/
#include "fifctl.h"
#include "fpgafifo.h"			/* Show current		*/


//#define		WUPforCHAR


void	fif_int_hdr(void);
int		fif_rxint_top(void);
int		fif_txint_top(void);

static int	wup_flg = 0;


void
fif_int_hdr(void){
	unsigned char	al;

	al = _fifo_status();
	if(  al & FIFORXDATA ) fif_rxint_top();
	if(!(al & FIFOTXFULL)) fif_txint_top();
}


void
fif_int_loop(void){
	unsigned char	al;

	for(;;){
		al = _fifo_status();
		if(  al & FIFORXDATA ){
			if(fif_rxint_top()) continue;
		}
		if(!(al & FIFOTXFULL)){
			if(fif_txint_top()) continue;
		}
		break;
	}
}


int
fif_rxint_top(void){
	unsigned short	ax;
	unsigned char	al;
	struct comh	*p;			/* ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ	*/

	if(fifrx[0].flag & RXPEND) return(0);

	p = &fifrx[0];
	if((((p->putp - p->getp) & p->buf_max)+ 1) >= p->buf_max){
		p->flag |= RXPEND;
		fpgamskflg |=  FPGA_RX_MSK;
		_fifo_msktxrx(fpgamskflg);
		return(0);
	}

	wup_flg = 0;
	al = _fifo_input();
	if(al == CR){
		++(fifrx[0].lcnt);
		wup_flg = 1;
	}
	ax = fifrx[0].putp;
	fifrx[0].buff[ax] = al;
	++ax;
	ax &= fifrx[0].buf_max;
	fifrx[0].putp =ax;

#ifdef	WUPforCHAR
	wup_tsk((ID)fif_rx_tsk[0]->acadr);
#else
	if(wup_flg) wup_tsk((ID)fif_rx_tsk[0]->acadr);
#endif

	return(1);
}


int
fif_txint_top(void){
	unsigned short	ax;
	unsigned char	al;

	if(!(fiftx[0].flag & TX_ING)) return(0);

	ax = fiftx[0].getp;
	if(ax == fiftx[0].putp){
		fiftx[0].flag &= ~TX_ING;
		fpgamskflg |= FPGA_TX_MSK;
		_fifo_msktxrx(fpgamskflg);
		return(0);
	}
	al = fiftx[0].buff[ax];
	_fifo_output(al);
	++ax;
	ax &= fiftx[0].buf_max;
	fiftx[0].getp = ax;
	return(1);
}


