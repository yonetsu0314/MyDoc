/*----------------------------------------------------------------------*
 * File name	: scuint.c	/ scu interrupt handler						*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/sciint.c 1.4		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic/RCS/sciint.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: sciint.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2005/01/24 １文字割り込みに変更
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/01/24 biosif経由とする。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/09/01 １行単位で割り込みが発生しない不具合修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA  CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* Function																*/
/*	SCUの割り込みﾊﾝﾄﾞﾗﾓｼﾞｭｰﾙ。											*/
/*----------------------------------------------------------------------*/
#include <kernel.h>
#include <machine.h>
#include <sh7615\sh7615.h>
#include "scictl.h"
#include "scictxt.h"

#define	SMR		0
#define	BRR		2
#define	SCR		4
#define	TDR		6
#define	SSR		8
#define	SSR2	10
#define	RDR		12

INTHDR sci1_errint_hdr(void);
INTHDR sci1_rxint_hdr(void);
INTHDR sci1_txint_hdr(void);
INTHDR sci2_errint_hdr(void);
INTHDR sci2_rxint_hdr(void);
INTHDR sci2_txint_hdr(void);

void	sci1_errint_top(void);
void	sci1_rxint_top(void);
void	sci1_txint_top(void);
void	sci2_errint_top(void);
void	sci2_rxint_top(void);
void	sci2_txint_top(void);

void	rx_int(int);
void	tx_int(int);
void	rxdtprog(int, unsigned char);

static int	wup_flg[SCIF_CH_MAX] = {0, 0};

#pragma interrupt(sci1_errint_hdr)
void sci1_errint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci1_errint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}
#pragma interrupt(sci1_rxint_hdr)
void sci1_rxint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci1_rxint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}
#pragma interrupt(sci1_txint_hdr)
void sci1_txint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci1_txint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}

#pragma interrupt(sci2_errint_hdr)
void sci2_errint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci2_errint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}
#pragma interrupt(sci2_rxint_hdr)
void sci2_rxint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci2_rxint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}
#pragma interrupt(sci2_txint_hdr)
void sci2_txint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	ent_int();					/* 割込みハンドラの開始 */
	sci2_txint_top();
	ret_int();					/* 割込みハンドラから復帰する */
}


void
sci1_errint_top(){
	static unsigned char	al;
	static unsigned short	ax;

	al = sfr_in(SCIF_SCFRDR1);
	ax = sfr_inw(SCIF_SC1SSR1);
	sfr_out(SCIF_SCSCR1,0x00);
//	wait0(1);
	sfr_outw(SCIF_SC1SSR1,0x0060);
	sfr_out(SCIF_SCSCR1,0x70);
}
void
sci1_rxint_top(){
	unsigned char	al;
	unsigned short	ax;

	wup_flg[SCIF_CH1] = 0;
	al = sfr_in(SCIF_SCFRDR1);

	if(al==CR){
		++(scrx[SCIF_CH1].lcnt);
		wup_flg[SCIF_CH1] = 1;
	}

	ax = scrx[SCIF_CH1].putp;
	scrx[SCIF_CH1].buff[ax] = al;
	++ax;
	ax &= scrx[SCIF_CH1].buf_max;
	scrx[SCIF_CH1].putp = ax;

	wup_tsk((ID)scif_rx_tsk[SCIF_CH1]->acadr);
	sfr_clrw(SCIF_SC1SSR1,0x0002);
}
void
sci1_txint_top(){
	unsigned short	ax;
	unsigned char	al;

	ax = sctx[SCIF_CH1].getp;
	if(ax == sctx[SCIF_CH1].putp) goto tx1;
	al = sctx[SCIF_CH1].buff[ax];
	sfr_out(SCIF_SCFTDR1,al);
	++ax;
	ax &= sctx[SCIF_CH1].buf_max;
	sctx[SCIF_CH1].getp = ax;
	sfr_clrw(SCIF_SC1SSR1,0x0020);
	return;
tx1:
	sctx[SCIF_CH1].flag &= ~TX_ING;
	sfr_clr(SCIF_SCSCR1,0x80);	/* initialize TIE */
	return;
}

void
sci2_errint_top(){
	static unsigned char	al;
	static unsigned short	ax;

	al = sfr_in(SCIF_SCFRDR2);
	ax = sfr_inw(SCIF_SC1SSR2);
	sfr_out(SCIF_SCSCR2,0x00);
//	wait0(1);
	sfr_outw(SCIF_SC1SSR2,0x0060);
	sfr_out(SCIF_SCSCR2,0x70);
}
void
sci2_rxint_top(){
	unsigned char	al;
	unsigned short	ax;

	wup_flg[SCIF_CH2] = 0;
	al = sfr_in(SCIF_SCFRDR2);

	if(al==CR){
		++(scrx[SCIF_CH2].lcnt);
		wup_flg[SCIF_CH2] = 1;
	}

	ax = scrx[SCIF_CH2].putp;
	scrx[SCIF_CH2].buff[ax] = al;
	++ax;
	ax &= scrx[SCIF_CH2].buf_max;
	scrx[SCIF_CH2].putp = ax;

	wup_tsk((ID)scif_rx_tsk[SCIF_CH2]->acadr);
	sfr_clrw(SCIF_SC1SSR2,0x0002);
}
void
sci2_txint_top(){
	unsigned short	ax;
	unsigned char	al;

	ax = sctx[SCIF_CH2].getp;
	if(ax == sctx[SCIF_CH2].putp) goto tx1;
	al = sctx[SCIF_CH2].buff[ax];
	sfr_out(SCIF_SCFTDR2,al);
	++ax;
	ax &= sctx[SCIF_CH2].buf_max;
	sctx[SCIF_CH2].getp = ax;
	sfr_clrw(SCIF_SC1SSR2,0x0020);
	return;
tx1:
	sctx[SCIF_CH2].flag &= ~TX_ING;
	sfr_clr(SCIF_SCSCR2,0x80);	/* initialize TIE */
	return;
}



