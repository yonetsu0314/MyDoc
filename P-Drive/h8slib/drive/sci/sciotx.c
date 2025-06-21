/*----------------------------------------------------------------------*
 * File name	: sciotx.c		/ sci transmit control					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/sciotx.c 1.1 1996/01/18 21:50:03 kabumoto Exp $
 *$Log: sciotx.c $
 * リビジョン 1.1  1996/01/18  21:50:03  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明  		                                                		*/
/*		送信動作を行う。内部処理として、送信中の判断はSCIのSCR.BIT		*/
/*		.TIE == 1で行う。送信割り込み許可／禁止のSCR.BIT.TEを使用		*/
/*		すると、シリアルポートの状態も変化するため、要注意。			*/
/*		1.送信中または送信中断状態でなく、且つ送信するﾃﾞｰﾀがあれば		*/
/*	      SCIのトランスミッタのレディを確認後、直接SCIにキャラクタ		*/
/*		  の書き込みを行う。											*/
/*		2.そうでない場合、return（送信割り込み内で処理される）。		*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch		: channel number										*/
/*		*p		: ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ										*/
/*																		*/
/* 戻り値																*/
/*		void	: 無し													*/
/*----------------------------------------------------------------------*/

#include	<custom\custom.h>					/* show custom			*/
#include	"scictxt.h"
#include	"scictl.h"

void
sci_io_tx(
	int				ch,							/* channel no.			*/
	struct comh		*p							/* ﾍｯﾀﾞｰへのﾎﾟｲﾝﾀｰ		*/
	)
{
	volatile struct st_sci*	cp;
	unsigned char			f;

	cp = sci_ctxt_tbl[ch];
	f = cp->SCR.BIT.TIE;
	cp->SCR.BIT.TIE = 0;					/* 送信割り込みdisable			*/
	if(!f) {
		if(cp->SSR.BIT.TDRE) {				/* 送信バッファが空であった場合 */
			cp->TDR = *(p->buff + p->getp);	/* 直接１バイト目を書き込む 	*/
			cp->SSR.BIT.TDRE = 0;			/* 送信セット 					*/
			p->getp = ++p->getp & p->buf_max;
		}
	}
	cp->SCR.BIT.TIE = 1;					/* 送信割り込みenable			*/
}
