/*----------------------------------------------------------------------*
 * File name	: fifsini.c	/ system initialize			*
 * Original	: p:/presto/shlib/drive/scu/RCS/scusini.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoscuc/rcs/fifsini.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
 * 
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
//#include	<biosif\scua.h>			/*  	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\fifdrv.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"fifctl.h"			/* Show current		*/
//#include	"fifctxt.h"			/*	"		*/
#undef		STORAGE


int
fif_sys_init(
	int			ch_no,
	const struct fifpk	*pkt,
	union aap		*rx_tsk
	)
{
	unsigned short	buf_max,		/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
			mask;			/* mask bit pattern	*/
//	int far		*phy_inf;		/* & to physical info	*/
	int		err;

	if(ch_no >= FIF_CH_MAX)					return(-1);
//	fif_ds.ifp = (int *)&fif_ds.acadr;
	if((pkt->siz_txbuf == 0)||(pkt->siz_rxbuf == 0))	return(-1);
	if((fif_rx_tsk[ch_no] = rx_tsk) == NULL)		return(-1);
	if(err = init_fif_rx(ch_no))				return(-1);
	if(err = init_fif_tx(ch_no))				return(-1);
	for(buf_max= (unsigned short)(pkt->siz_txbuf-1), 
		mask =(unsigned short)0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((fiftx[ch_no].buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			fiftx[ch_no].buf_max = buf_max;
		}
	}
	for(buf_max= (unsigned short)(pkt->siz_txbuf-1),
		mask =(unsigned short)0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((fifrx[ch_no].buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			fifrx[ch_no].buf_max = buf_max;
		}
	}
//	if((fif_icu_inf[ch_no].i =
//	    (short)_icu_dev_hunt(fif_int_hdr[ch_no])) == -1)	return(-1);
//	else {
		/* エンディアンに関係無く uc[0]:下位 uc[1]:上位 */
//		fif_icu_inf[ch_no].uc[0] = (unsigned char)(fif_icu_inf[ch_no].i & (short)0x00ff);
//		fif_icu_inf[ch_no].uc[1] = (unsigned char)(fif_icu_inf[ch_no].i & (short)0xff00);
//		fif_ctxt_tbl[ch_no].ipfw = _get_ipfw((int)fif_icu_inf[ch_no].uc[1]);
//		if((phy_inf = _fif_init(ch_no, pkt->char_len, pkt->stp_len,
//				     pkt->pari_sel)) == NULL)	return(-1);
//		fif_ctxt_tbl[ch_no].io_base = *phy_inf;
//	}
	iodrv_open[__FIFO_-1] = fifo_open;
	return(0);
}
