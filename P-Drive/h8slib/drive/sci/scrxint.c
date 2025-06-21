/*----------------------------------------------------------------------*
 * File name	: scrxint.c		/ sci RX interrupt handler				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scrxint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scrxint.c $
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
/*		SCI受信割り込み処理												*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		write_fifo														*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*p		: 受信管理用ﾍｯﾀﾞへのﾎﾟｲﾝﾀ								*/
/*		c		: 受信ﾃﾞｰﾀ												*/
/*																		*/
/* 戻り値																*/
/*		int		: N	…	text受信完了									*/
/*				  0	…	text完了していない								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		受信データをリングバッファに書き込む。関数内は割り込み禁止		*/
/*		状態を前提に処理が行われる。									*/
/*----------------------------------------------------------------------*/

static int
write_fifo(
	struct comh		*p,
	unsigned char	c
	)
{
	unsigned char		*rp;				/* ring bufferへのﾎﾟｲﾝﾀｰ	*/
	unsigned short		i, j, max;

	rp = p->buff;
	i = p->putp;
	max = p->buf_max;
	switch(c)	{
		case DEL:
		case BS:
				if(i != p->getp) {
					i = --i & max;
					if(*(rp+i) != CR)  p->putp = i;
				}
				break;
		case CTLX:
				while(i != p->getp) {
					i = --i & max;
					if(*(rp+ i) != CR) {
						p->putp = i;  break;
					}
				}
				*(rp+ i) = c;
				p->putp = ++i & max;
				if(i == p->getp) {
					p->putp = ++i & max;
					*(rp+ i) = CR;					return(++p->lcnt);
				}
				c = CR;
		case CR:
		case LF:
		case HT:
		default:
				*(rp+ i) = c;
				p->putp = ++i & max;
				if(c == CR)							return(++p->lcnt);
				else {
					if(i == p->getp) {
						p->putp = ++i & max;
						*(rp+ i) = CR;				return(++p->lcnt);
					}
				}
				break;
	}
	return(0);
}

/*----------------------------------------------------------------------*/
/*		sci_rxint_hdr													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		void		: 無し												*/
/*																		*/
/* 戻り値																*/
/*		void		: 無し												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		受信割り込み関数												*/
/*		受信データをリングバッファに書き込む							*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_rxint_hdr)				/* SCI-ch0割り込みﾊﾝﾄﾞﾗ	*/
void	sci0_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH0];
	i = write_fifo(&scrx[CH0], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}


#pragma interrupt(sci1_rxint_hdr)				/* SCI-ch1割り込みﾊﾝﾄﾞﾗ	*/
void	sci1_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH1];
	i = write_fifo(&scrx[CH1], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}

#pragma interrupt(sci2_rxint_hdr)				/* SCI-ch2割り込みﾊﾝﾄﾞﾗ	*/
void	sci2_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH2];
	i = write_fifo(&scrx[CH2], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}
