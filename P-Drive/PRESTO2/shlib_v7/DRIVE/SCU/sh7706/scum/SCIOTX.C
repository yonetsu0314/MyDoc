/*----------------------------------------------------------------------*
 * File name	: sciotx.c	/ scu transmit control			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scum/RCS/sciotx.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: sciotx.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/05/02 16:30
 * SCIFの送信データエンプティのチェックビットの誤りを修正する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_scu/rcs/sciotx.c 2.1 1994/09/05 14:21:31 kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	送信動作またはフリー･バッファの確保を行う。			*/
/*	1.ﾌﾘｰ･ﾊﾞｯﾌｧが要求されている場合、				*/
/*	    a).送信中または送信中断状態であれば、待ち状態に入り空き	*/
/*	       がn以上なるまで1.を繰り返す。				*/
/*	2.そうで無い場合、						*/
/*	    c).送信中または送信中断状態でなく、且つ送信するﾃﾞｰﾀがあ	*/
/*	       ればSCUのトランスミッタのレディを確認後、直接SCUにキ	*/
/*	       ャラクタの書き込みを行う。				*/
/*	    d).そうでない場合、return。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*p		: ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ				*/
/*	n		: ﾌﾘｰ･ﾊﾞｯﾌｧの要求ﾊﾞｲﾄ数				*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 要求ﾊﾞｲﾄ数				*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* Show intrpt		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\scua.h>			/* Show bios		*/
#include	"scuctl.h"			/* Show current		*/

#define	SCI_TBRDY	(0x80)
#define	SCIF_TBRDY	(0x60)

int
scu_io_tx(
	int		ch,			/* channel number	*/
	struct comh	*p,			/* ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ	*/
	int		byte			/* ﾌﾘｰﾊﾞｯﾌｧの要求ﾊﾞｲﾄ数	*/
	)
{
	static long	buf_ck_interval = 20;	/* 20msのｲﾝﾀｰﾊﾞﾙ	*/
	static long	tbrdy_ck_interval = 2;	/* 2msのｲﾝﾀｰﾊﾞﾙ		*/
	int		err = 0;
	int		rdy;

	_disable();
	if(byte) {
		do {
			if(!(p->flag & (TX_ING|PENDING)))  break;
			if(err = HLT_TSK( &buf_ck_interval))  goto fail;
		} while((byte +((p->putp- p->getp) & p->buf_max))
			>= p->buf_max);
	}
	else {
		if((!(p->flag & (TX_ING|PENDING))) && (p->putp != p->getp)) {
			if(ch==0){
				rdy = SCI_TBRDY;
			} else {
				rdy = SCIF_TBRDY;
			}
			while(!(_scu_sst(ch) & rdy)) {
				if(err = HLT_TSK( &tbrdy_ck_interval))
					goto fail;
			}
			_scu_putc(ch, (int)*(p->buff + p->getp));
			p->getp = (unsigned short)(++p->getp & p->buf_max);
			p->flag |= TX_ING;
		}
	}
	_enable();
fail:	if(err)							return(-1);
	return(byte);
}
