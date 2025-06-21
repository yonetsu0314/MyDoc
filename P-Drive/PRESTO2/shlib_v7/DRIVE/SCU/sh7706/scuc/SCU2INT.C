/*----------------------------------------------------------------------*
 * File name	: scu2int.c	/ scu interrupt handler			*
 * Original	: 
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scuc/RCS/scu2int.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: scu2int.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2004/06/21 18:00
 * 前リビジョンで受信エラー時の処理を修正したが、受信バッファフルをクリア
 * していないために、受信割り込みが解除されない不具合修正。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2004/06/21
 * 受信エラー時の解除処理が正しくなくエラー解除できないため、永遠にエラー解除
 * 割り込みから抜けない不具合修正。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/13 １文字受信で割り込みに変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* Function								*/
/*	SCUの割り込みﾊﾝﾄﾞﾗﾓｼﾞｭｰﾙ。					*/
/*----------------------------------------------------------------------*/
#include <kernel.h>
#include <machine.h>
#include <sh7706\7706.h>
#include "scuctl.h"
#include "scuctxt.h"

#define	E_FER	((unsigned short)0x08)
#define	E_PER	((unsigned short)0x04)

void	scu2_errint_top(void);
void	scu2_rxint_top(void);
void	scu2_txint_top(void);

static int	wup_flg = 0;

void
scu2_errint_hdr(void)
{
	scu2_errint_top();
}

void
scu2_rxint_hdr(void)
{
	scu2_rxint_top();
}

void
scu2_txint_hdr(void)
{
	scu2_txint_top();
}

static void
wait01(void)
{
	int	i = 750;
	for(; i; i--);
}

void
scu2_errint_top(void)
{
	static unsigned char	al;

	al = SCIF.SCFRDR;
	SCIF.SCSSR.BIT.ER = 0;
	SCIF.SCSSR.BIT.DR = 0;
	SCIF.SCSSR.BIT.BRK = 0;
	SCIF.SCSSR.BIT.RDF = 0;
}

void
scu2_rxint_top(void)
{
	unsigned char	al;
	unsigned short	ax;

	ax = SCIF.SCSSR.WORD;
	if(ax & (E_FER | E_PER)){
		scu2_errint_top();
		return;
	}
	wup_flg = 0;
	al = SCIF.SCFRDR;

	if(al == CR){
		++(scrx[1].lcnt);
		wup_flg = 1;
	}

	ax = scrx[1].putp;
	scrx[1].buff[ax] = al;
	++ax;
	ax &= scrx[1].buf_max;
	scrx[1].putp =ax;

	wup_tsk((ID)scu_rx_tsk[1]->acadr);
	SCIF.SCSSR.BIT.RDF = (unsigned char)0;

}
void
scu2_txint_top(void)
{
	unsigned short	ax;
	unsigned char	al;

	ax = sctx[1].getp;
	if(ax == sctx[1].putp) goto tx1;
	al = sctx[1].buff[ax];
	SCIF.SCFTDR = al;
	++ax;
	ax &= sctx[1].buf_max;
	sctx[1].getp = ax;
	SCIF.SCSSR.BIT.TDFE = (unsigned char)0;
	return;
tx1:
	sctx[1].flag &= ~TX_ING;
	SCIF.SCSCR.BIT.TIE = 0;

	return;
}
