/*----------------------------------------------------------------------*
 * File name	: sciotx.c	/ scu transmit control						*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/sciotx.c 1.2
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic2/RCS/sciotx.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: sciotx.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/03/24
 * biosif\scua.hで定義しているTBRDYがsh7706用のため、sciotx.cで再定義する。
 * #undef  TBRDY
 * #define TBRDY   (0x20)  TXバッファー書き込み可
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/25 biosif経由とする。
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
/*	送信動作またはフリー･バッファの確保を行う。							*/
/*	1.ﾌﾘｰ･ﾊﾞｯﾌｧが要求されている場合、									*/
/*	    a).送信中または送信中断状態であれば、待ち状態に入り空き			*/
/*	       がn以上なるまで1.を繰り返す。								*/
/*	2.そうで無い場合、													*/
/*	    c).送信中または送信中断状態でなく、且つ送信するﾃﾞｰﾀがあ			*/
/*	       ればSCUのトランスミッタのレディを確認後、直接SCUにキ			*/
/*	       ャラクタの書き込みを行う。									*/
/*	    d).そうでない場合、return。										*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	*p		: ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ											*/
/*	n		: ﾌﾘｰ･ﾊﾞｯﾌｧの要求ﾊﾞｲﾄ数										*/
/*																		*/
/* 戻り値																*/
/*	int		: n	･･･ 要求ﾊﾞｲﾄ数											*/
/*			  -1	･･･ error											*/
/*----------------------------------------------------------------------*/

#include	<dos.h>							/* MS-C std					*/
#include	<rxif\rx116.h>					/* Show xlib				*/
#include	<biosif\scua.h>					/* Show bios				*/
#include	"scictl.h"						/* Show current				*/

#define		SCIF_CH1	0					/* SCIF Channel 1			*/
#define		SCIF_CH2	1					/* SCIF Channel 2			*/

#undef	TBRDY
#define	TBRDY		(0x20)					/* TXバッファー書き込み可	*/

int scif1_io_putc(int data);
int scif2_io_putc(int data);

int
scif_io_tx(
	int				ch,						/* channel no.				*/
	struct comh 	*p,						/* ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ			*/
	int				byte					/* ﾌﾘｰﾊﾞｯﾌｧの要求ﾊﾞｲﾄ数		*/
	)
{
	static long	buf_ck_interval = 20;		/* 20msのｲﾝﾀｰﾊﾞﾙ			*/
	static long	tbrdy_ck_interval = 2;		/* 2msのｲﾝﾀｰﾊﾞﾙ				*/
	int		err = 0;

	_disable();
	if(byte) {
		do {
			if(!(p->flag & (TX_ING|PENDING)))  break;
			if(err = HLT_TSK( &buf_ck_interval))  goto fail;
		} while((byte +((p->putp- p->getp) & p->buf_max)) >= p->buf_max);
	}
	else {
		if((!(p->flag & (TX_ING|PENDING))) && (p->putp != p->getp)) {
			if (ch == SCIF_CH1) {
				while(!(_scu_sst(ch) & TBRDY)) {
					if(err = HLT_TSK( &tbrdy_ck_interval))
						goto fail;
				}
				_scu_putc(ch, (int)*(p->buff + p->getp));
				p->getp = ++p->getp & p->buf_max;
				p->flag |= TX_ING;
			} else {
				while(!(_scu_sst(ch) & TBRDY)) {
					if(err = HLT_TSK( &tbrdy_ck_interval))
						goto fail;
				}
				_scu_putc(ch, (int)*(p->buff + p->getp));
				p->getp = ++p->getp & p->buf_max;
				p->flag |= TX_ING;
			}
		}
	}
fail:
	_enable();
	if(err)							return(-1);
	return(byte);
}
