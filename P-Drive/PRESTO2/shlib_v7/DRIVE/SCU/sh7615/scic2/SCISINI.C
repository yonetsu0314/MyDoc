/*----------------------------------------------------------------------*
 * File name	: scisini.c		/ system initialize						*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scisini.c 1.2
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic2/RCS/scisini.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scisini.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/24 biosif経由とする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
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
#include	<biosif\scua.h>							/*  "	bios		*/
#include	<drive\iofsys.h>						/* Show drive		*/
#include	<drive\sh7615\scidrv.h>					/*	"				*/

INTHDR sci1_errint_hdr(void);
INTHDR sci1_rxint_hdr(void);
INTHDR sci1_txint_hdr(void);
INTHDR sci2_errint_hdr(void);
INTHDR sci2_rxint_hdr(void);
INTHDR sci2_txint_hdr(void);

#define		STORAGE									/* 確保/外部参照	*/
#include	"scictxt.h"
#include	"scictl.h"
#undef		STORAGE

int
scif_sys_init(
	int					ch,
	const struct scipk	*pkt,
	union aap 			*rx_tsk
	)
{
	unsigned short	buf_max,						/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
					mask;							/* mask bit pattern	*/
	int				*phy_inf;						/* & to physical info	*/
	int				err;
	int				ret = 0;						/* 再初期化防止 flag	*/

	scif_ds.ifp = (int *)&(scif_ds.acadr);
	if((pkt->siz_txbuf == 0)||(pkt->siz_rxbuf == 0))	return(-1);
	if(scif_rx_tsk[ch] != NULL) {
		ret = 1;									/*** 再設定 ***/
		if(rx_tsk == NULL) rx_tsk = scif_rx_tsk[ch];/* 受信タスク変更しない */
	}
	if (ch == SCIF_CH1) {
		if((scif_rx_tsk[ch] = rx_tsk) == NULL)			return(-1);
		if(err = init_scif1_rx())						return(-1);
		if(err = init_scif1_tx())						return(-1);
	} else if (ch == SCIF_CH2) {
		if((scif_rx_tsk[ch] = rx_tsk) == NULL)			return(-1);
		if(err = init_scif2_rx())						return(-1);
		if(err = init_scif2_tx())						return(-1);
	} else return(-1);

	if( !ret ) {
		for(buf_max = pkt->siz_txbuf-1, mask = 0x8000; mask; mask >>= 1) {
			if(buf_max & mask) {
				while(mask >>= 1)  buf_max |= mask;
				if((sctx[ch].buff = malloc(buf_max+1)) == NULL)		return(-1);
				sctx[ch].buf_max = buf_max;
			}
		}
		for(buf_max= pkt->siz_rxbuf-1, mask =0x8000; mask; mask >>= 1) {
			if(buf_max & mask) {
				while(mask >>= 1)  buf_max |= mask;
				if((scrx[ch].buff = malloc(buf_max+1)) == NULL)		return(-1);
				scrx[ch].buf_max = buf_max;
			}
		}
	}

	if (ch == SCIF_CH1) {
		if((scif_icu_inf[ch].i = _icu_dev_hunt(scif_int_hdr[ch])) == -1) return(-1);
		scif_ctxt_tbl[ch].ipfw = _get_ipfw(scif_icu_inf[ch].i);
		_scu_baud(ch, pkt->baud);
		if((phy_inf = _scu_init(ch, pkt->char_len, pkt->stp_len,
											pkt->pari_sel)) == NULL)		return(-1);
	} else if (ch == SCIF_CH2) {
		if((scif_icu_inf[ch].i = _icu_dev_hunt(scif_int_hdr[ch])) == -1) return(-1);
		scif_ctxt_tbl[ch].ipfw = _get_ipfw(scif_icu_inf[ch].i);
		_scu_baud(ch, pkt->baud);
		if((phy_inf = _scu_init(ch, pkt->char_len, pkt->stp_len,
											 pkt->pari_sel)) == NULL)	return(-1);
	} else return(-1);
	scif_ctxt_tbl[ch].io_base = (unsigned short)phy_inf;

	iodrv_open[__SCU_-1] = (void *)scif_open;
	return( ret );
}
