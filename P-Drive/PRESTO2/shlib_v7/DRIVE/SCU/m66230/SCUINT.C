/*----------------------------------------------------------------------*
 * File name	: scuint.c	/ m66230 interrupt handler					*
 * Original	: 
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scuint.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuint.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/02/03
 * エラーのチェックを受信処理で行うように変更
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/02/01 割り込みハンドラ内の記述を変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scu1int.c 1.4 1970/01/01
 *Log: scu1int.c
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/08/05 ヘッダファイル7706.hのインクルード先を変更する。
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA  CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* Function																*/
/*	M66230の割り込みﾊﾝﾄﾞﾗﾓｼﾞｭｰﾙ。										*/
/*----------------------------------------------------------------------*/
#include 	<kernel.h>
#include 	<machine.h>
#include	<biosif\m66230.h>					/* Show bios			*/
#include 	"scuctl.h"
#include 	"scuctxt.h"


void	m66230_errint(int);
void	m66230_rxint(int);
void	m66230_txint(int);

static int	wup_flg[M66230_CH_MAX] = {0};

void
m66230_int_hdr0(void)
{
	int	st;
	
	st = _m66230_gst(0);

	if(st & (RXBFULL | RXBPE)){
		m66230_rxint(0);
	}
	if((st & (TXBEMP | TXEMP)) == (TXBEMP | TXEMP)){
		m66230_txint(0);
	}
}

void
m66230_int_hdr1(void)
{
	int	st;
	
	st = _m66230_gst(1);

	if(st & (RXBFULL | RXBPE)){
		m66230_rxint(1);
	}
	if((st & (TXBEMP | TXEMP)) == (TXBEMP | TXEMP)){
		m66230_txint(1);
	}
}

static void
wait01(void)
{
	int	i = 750;
	for(; i; i--);
}

void
m66230_errint(int ch)
{
	_m66230_sst(ch, ERR_CLR);
	wait01();
	for(;;){
		if(!(_m66230_gst(ch) & ALL_ERR)) {
			break;
		}
		wait01();
	}
	wait01();
	_m66230_sst(ch, 0xff);
	return;
}

void
m66230_rxint(int ch)
{
	unsigned char	al;
	unsigned short	ax;
	int				st;

	wup_flg[ch] = 0;
	al = (unsigned char)_m66230_getc(ch);

	st = _m66230_gst(ch);
	if(st & ALL_ERR){
		m66230_errint(ch);
		return;
	}

	if(al == CR){
		++(m66230rx[ch].lcnt);
		wup_flg[ch] = 1;
	}

	ax = m66230rx[ch].putp;
	m66230rx[ch].buff[ax] = al;
	++ax;
	ax &= m66230rx[ch].buf_max;
	m66230rx[ch].putp =ax;

	if(wup_flg[ch]) wup_tsk((ID)m66230_rx_tsk[ch]->acadr);
	return;
}
void
m66230_txint(int ch)
{
	unsigned short	ax;
	unsigned char	al;

	ax = m66230tx[ch].getp;
	if(ax == m66230tx[ch].putp) goto tx1;
	al = m66230tx[ch].buff[ax];
	_m66230_putc(ch, (int)al);
	++ax;
	ax &= m66230tx[ch].buf_max;
	m66230tx[ch].getp = ax;
	return;
tx1:
	m66230tx[ch].flag &= ~TX_ING;
	_m66230_sst(ch, TXDIS);				/*	送信割り込み禁止	*/

	return;
}
