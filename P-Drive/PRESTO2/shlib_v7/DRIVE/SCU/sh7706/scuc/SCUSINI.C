/*----------------------------------------------------------------------*
 * File name	: scusini.c	/ system initialize			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scuc/RCS/scusini.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scusini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_scu/rcs/scusini.c 2.1 1994/09/05 14:25:41 kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	SCUのOS環境設定、デバイスの初期設定を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ch_no		: SCUﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*pkt		: ﾌﾞｰﾄ･ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ			*/
/*	    typedef struct scupk {					*/
/*		int		char_len	･･･ ｷｬﾗｸﾀ長		*/
/*		int		stp_len		･･･ ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数	*/
/*		int		pari_sel	･･･ ﾊﾟﾘﾃｨｰ選択		*/
/*		unsigned short	siz_txbuf	･･･ 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
/*		unsigned short	siz_rxbuf	･･･ 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
/*	    } SCUPK;							*/
/*									*/
/* 	*rx_tsk		: ｱｸｾｽ･ｱﾄﾞﾚｽへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<biosif\icu.h>			/*  "	bios		*/
#include	<biosif\scua.h>			/*  	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\scudrv.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"scuctl.h"			/* Show current		*/
#include	"scuctxt.h"			/*	"		*/
#undef		STORAGE

int
scu_sys_init(
	int			ch_no,
	const struct scupk	*pkt,
	union aap		*rx_tsk
	)
{
	unsigned short	buf_max,		/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
			mask;			/* mask bit pattern	*/
	int far		*phy_inf;		/* & to physical info	*/
	int		err;

	if(ch_no >= SCU_CH_MAX)					return(-1);
	scu_ds.ifp = (int *)&scu_ds.acadr;
	if((pkt->siz_txbuf == 0)||(pkt->siz_rxbuf == 0))	return(-1);
	if((scu_rx_tsk[ch_no] = rx_tsk) == NULL)		return(-1);
	if(err = init_scu_rx(ch_no))				return(-1);
	if(err = init_scu_tx(ch_no))				return(-1);
	for(buf_max= (unsigned short)(pkt->siz_txbuf-1), 
		mask =(unsigned short)0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((sctx[ch_no].buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			sctx[ch_no].buf_max = buf_max;
		}
	}
	for(buf_max= (unsigned short)(pkt->siz_txbuf-1),
		mask =(unsigned short)0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((scrx[ch_no].buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			scrx[ch_no].buf_max = buf_max;
		}
	}
	if((scu_icu_inf[ch_no].i =
	    (short)_icu_dev_hunt(scu_int_hdr[ch_no])) == -1)	return(-1);
	else {
		/* エンディアンに関係無く uc[0]:下位 uc[1]:上位 */
		scu_icu_inf[ch_no].uc[0] = (unsigned char)(scu_icu_inf[ch_no].i & (short)0x00ff);
		scu_icu_inf[ch_no].uc[1] = (unsigned char)(scu_icu_inf[ch_no].i & (short)0xff00);
		scu_ctxt_tbl[ch_no].ipfw = _get_ipfw((int)scu_icu_inf[ch_no].uc[1]);
		if((phy_inf = _scu_init(ch_no, pkt->char_len, pkt->stp_len,
				     pkt->pari_sel)) == NULL)	return(-1);
		scu_ctxt_tbl[ch_no].io_base = *phy_inf;
	}
	iodrv_open[__SCU_-1] = scu_open;
	return(0);
}
