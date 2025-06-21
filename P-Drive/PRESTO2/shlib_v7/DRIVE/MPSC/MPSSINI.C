/*----------------------------------------------------------------------*
 * File name	: mpssini.c	/ system initialize			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/mpsc/RCS/mpssini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mpssini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	MPSCのOS環境設定、BIOSｺﾝﾃｷｽﾄの生成を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ch_no		: MPSCﾁｬﾈﾙ･ﾅﾝﾊﾞｰ				*/
/*	*pkt		: ﾌﾞｰﾄ･ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ			*/
/*	    typedef struct mpspk {					*/
/*		int		char_len	･･･ ｷｬﾗｸﾀ長		*/
/*		int		stp_len		･･･ ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数	*/
/*		int		pari_sel	･･･ ﾊﾟﾘﾃｨｰ選択		*/
/*		int		baud,		･･･ 転送ｽﾋﾟｰﾄﾞ		*/
/*		unsigned short	siz_txbuf	･･･ 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
/*		unsigned short	siz_rxbuf	･･･ 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
/*	    } MPSPK;							*/
/*									*/
/* 	*rx_tsk		: ｱｸｾｽ･ｱﾄﾞﾚｽへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*----------------------------------------------------------------------*/
#include	<drive\mpscdrv.h>

int
mpsc_sys_init(
	int			ch_no,
	const struct mpspk	*pkt,
	union aap		*rx_tsk
	)
{
	return(0);
}

