/*----------------------------------------------------------------------*
 * File name	: scisini.c		/ system initialize						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scisini.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scisini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993-2001, ISHIDA  CO., LTD.				*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		デバイスの初期設定を行う。SCIの設定モードは、調歩同期，内部		*/
/*		クロックに設定される。											*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		ch		: SCIﾁｬﾈﾙ･ﾅﾝﾊﾞｰ											*/
/*		*pkt	: ﾌﾞｰﾄ･ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ							*/
/*		    typedef struct scipk {										*/
/*			int				char_len	･･･ ｷｬﾗｸﾀ長						*/
/*			int				stp_len		･･･ ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数				*/
/*			int				pari_sel	･･･ ﾊﾟﾘﾃｨｰ選択					*/
/*			int				baud;		･･･ ﾋﾞｯﾄﾚｰﾄ						*/
/*			unsigned short	siz_txbuf	･･･ 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ				*/
/*			unsigned short	siz_rxbuf	･･･ 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ				*/
/*		    } SCIPK;													*/
/*																		*/
/* 戻り値																*/
/*		int		: 0	･･･ 正常終了										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>								/* ANSI std			*/
#include	<stdlib.h>								/*	"				*/
#include	<rom\handle.h>							/* show xlib		*/
#include	<drive\iofsys.h>						/* Show drive		*/
#include	<drive\scidrv.h>						/*	"				*/

#define		STORAGE									/* 確保/外部参照	*/
#include	"scictxt.h"
#include	"scictl.h"
#undef		STORAGE

static const unsigned char	chr_len[] = {	/* ｷｬﾗｸﾀ長のﾃｰﾌﾞﾙ			*/
			BIT7, BIT8
};
static const unsigned char	stp_len[] = {	/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ長のﾃｰﾌﾞﾙ		*/
			STOP1, STOP2
};
static const unsigned char	parity[] = {	/* ﾊﾟﾘﾃｨ選択ﾃｰﾌﾞﾙ			*/
			NON, ODD, EVEN
};
struct cksci {
	unsigned char	n;						/* ｸﾛｯｸｿｰｽ/1/4/16/64		*/
	unsigned char	br;						/* ﾋﾞｯﾄﾚｰﾄﾚｼﾞｽﾀ/N			*/
}											/* Clock at 19.6608 		*/
static const bsci[] = {						/* baud rate 設定用テーブル */
			1,	127,						/* 0 :	  1200 BPS			*/
			0,	255,						/* 1 :	  2400 BPS			*/
			0,	127,						/* 2 :	  4800 BPS			*/
			0,	63,							/* 3 :	  9600 BPS			*/
			0,	31,							/* 4 :	 19.2K BPS			*/
			0,	15							/* 5 :	 38.4K BPS			*/
};

int
sci_sys_init(
	int					ch,
	const struct scipk	*pkt
	)
{
	unsigned short			buf_max,				/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
							mask;					/* mask bit pattern	*/
	volatile struct st_sci	*p;

	if((pkt->siz_txbuf == 0)||(pkt->siz_rxbuf == 0))		return(-1);
	if(pkt->baud > sizeof(bsci)/sizeof(bsci[0]))			return(-1);
	for(buf_max= pkt->siz_txbuf-1, mask =0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((sctx[ch].buff = malloc(buf_max+1))== NULL)	return(-1);
			sctx[ch].buf_max = buf_max;
		}
	}
	for(buf_max= pkt->siz_txbuf-1, mask =0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((scrx[ch].buff = malloc(buf_max+1))== NULL)	return(-1);
			scrx[ch].buf_max = buf_max;
		}
	}
	p = sci_ctxt_tbl[ch];
	p->SCR.BYTE = 0;							/* clkｿｰｽ設定			*/
	p->BRR = bsci[pkt->baud].br;
	p->SMR.BYTE = (parity[pkt->pari_sel] | stp_len[pkt->stp_len]
				  | chr_len[pkt->char_len] | bsci[pkt->baud].n);
	p->SCR.BYTE = RIE_BP|TXE_BP|RXE_BP;			/* RxINT ,Tx/Rx enable */
	p->SSR.BIT.RDRF = 0;

	iodrv_open[__SCI_-1] = sci_open;
	return(0);
}
