/*----------------------------------------------------------------------*
 * File name	: sctxint.c		/ sci TX interrupt handler				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/sctxint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: sctxint.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                         		                         		*/
/*		SCI送信割り込み処理												*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		read_fifo														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*p		: 送信管理用ﾍｯﾀﾞへのﾎﾟｲﾝﾀ								*/
/*																		*/
/* 戻り値																*/
/*		int 	:	読み出したキャラクタ （正の値）						*/
/*					バッファが空の時は 負の値							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		送信用リング･バッファの内容を１文字読み出す。					*/
/*		関数内は割り込み禁止を前提に処理が行われる。					*/
/*----------------------------------------------------------------------*/

static int
read_fifo(
	struct comh		*p
	)
{
	unsigned short	i;
	unsigned char	c;

	if((i = p->getp) == p->putp)					return(-1);
	c = *(p->buff+ i);
	p->getp = (++i & p->buf_max);
	return((int)c);
}

/*----------------------------------------------------------------------*/
/*		sci_txint_hdr													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		void		: 無し												*/
/*																		*/
/* 戻り値																*/
/*		void		: 無し												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		送信割り込み関数												*/
/*		リングバッファに書き込まれたデータを送信する					*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_txint_hdr)				/* SCI-ch0割り込みﾊﾝﾄﾞﾗ	*/
void	sci0_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH0];
	if((i = read_fifo(&sctx[CH0])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}


#pragma interrupt(sci1_txint_hdr)				/* SCI-ch1割り込みﾊﾝﾄﾞﾗ	*/
void	sci1_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH1];
	if((i = read_fifo(&sctx[CH1])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}

#pragma interrupt(sci2_txint_hdr)				/* SCI-ch2割り込みﾊﾝﾄﾞﾗ	*/
void	sci2_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH2];
	if((i = read_fifo(&sctx[CH2])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}
