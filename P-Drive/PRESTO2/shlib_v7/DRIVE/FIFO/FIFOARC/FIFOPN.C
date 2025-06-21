/*----------------------------------------------------------------------*
 * File name	: fifopn.c	/ fifo open				*
 * Original	: p:/presto/shlib/drive/scu/RCS/scuopn.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoarc/rcs/fifopn.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifopn.c $
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
/*	FIFOをオープンする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0						*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<intrpt.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	<biosif\fifo.h>		/*	"		*/
#include	"fifctl.h"			/* Show current		*/
#include	"fpgafifo.h"

int
fifo_open(
	int		ch,
	int		mode
	)
{
	_disable();
	fpgamskflg &= ~FPGA_RX_MSK;
	fpgamskflg |=  FPGA_TX_MSK;
	_fifo_msktxrx(fpgamskflg);
	fif_mode = mode;
	fiftx[ch].putp = fiftx[ch].getp = fiftx[ch].lcnt = (unsigned short)0;
	fifrx[ch].putp = fifrx[ch].getp = fifrx[ch].lcnt = (unsigned short)0;
	iodrv_read[__FIFO_-1] = fifo_rx;
	iodrv_write[__FIFO_-1] = fifo_tx;
	iodrv_close[__FIFO_-1] = fifo_close;
	_enable();
	return(0);
}
