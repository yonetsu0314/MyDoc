/*======================================================================
 * File name    : pktdef.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/mhic/main/ccw/rcs/pktdef.h 1.2 1998/09/01 14:20:11 yonetsu Exp $
 *$Log: pktdef.h $
 * リビジョン 1.2  1998/09/01  14:20:11  yonetsu
 * 予約交信用デフォルト通信パラメータ
 * 
 * リビジョン 1.1  1997/05/28  14:17:02  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_PKTDEF_H_
#define	_PKTDEF_H_

#include <drive\serial.h>
#include <drive\sh7615\scidrv.h>
#include <drive\mpsdrva.h>
#include <drive\comdrva.h>

static const int	rate_table[] = {
	C2400BPS, C4800BPS,		/* 2400BPS 4800BPS	*/
	C9600BPS, C19KBPS		/* 9600BPS 19.2KBPS	*/
};

static const struct scipk	con0pkt = {
	C8BITLEN,			/* キャラクタ長		*/
	C1STPBIT,           		/* stop bit		*/
	ODD_PARI,            		/* パリティー選択 	*/
	C9600BPS,			/* 9600BPS 		*/
	1024,				/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	1024				/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
};

static const struct scipk	con1pkt = {
	C8BITLEN,			/* キャラクタ長		*/
	C1STPBIT,           		/* stop bit		*/
	ODD_PARI,            		/* パリティー選択 	*/
	C9600BPS,			/* 9600BPS 		*/
	1024,				/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	1024				/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
};

/* MHIC <---> HIF 通信ライン */

static struct mpspk	aux0pkt = {
	C8BITLEN,			/* キャラクタ長		*/
	C1STPBIT,           		/* stop bit		*/
	ODD_PARI,            		/* パリティー選択	*/
	C9600BPS,			/* 9600 KBPS		*/
	512,				/* 送信バッファ・サイズ	*/
	512,				/* 受信バッファ・サイズ	*/
	FULL_DUPLEX,
	MODEM_CTRL_OFF
};


static struct mpspk	aux1pkt = {
	C8BITLEN,			/* キャラクタ長		*/
	C1STPBIT,           		/* stop bit		*/
	ODD_PARI,            		/* パリティー選択	*/
	C9600BPS,			/* 9600KBPS		*/
	512,				/* 送信バッファ・サイズ	*/
	512,				/* 受信バッファ・サイズ	*/
	FULL_DUPLEX,
	MODEM_CTRL_OFF
};


static struct compk	hstpkt = {
	100,				/* ノード番号		*/
	C2500KBPS,			/* 転送スピード		*/
	1024,				/* 受信バッファ・サイズ	*/
};

static struct compk	hicpkt = {
	100,				/* ノード番号		*/
	C2500KBPS,          		/* 転送スピード		*/
	1024,              		/* 受信バッファ・サイズ	*/
};

static struct compk	rcupkt = {
	100,				/* ノード番号		*/
	C2500KBPS,          		/* 転送スピード		*/
	1024,               		/* 受信バッファ・サイズ	*/
};

#endif
