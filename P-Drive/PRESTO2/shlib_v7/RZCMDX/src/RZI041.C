/*======================================================================
 * File name    : rzi041.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（応答）テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi041.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi041.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi041.c 1.1 1996/12/19 10:05:45 hiroki
 *Log: rzi041.c
 * リビジョン 1.1  1996/12/19  10:05:45  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show rzcmd		*/
#define		STORAGE
#include	<rzcmd\rzi041.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_shift_data						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	シフトデータテキスト（応答）を構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_shift_data(
	struct fmt_shift_data	*buf
	)
{
	PI410	*p;
	int	i;

	rzi041s.ch_id = ascii_hex(buf->ch_id);
	rzi041s.all_num   = htoc((char *)buf->all_num);
	rzi041s.data_page = htoc((char *)buf->data_page);
	rzi041s.data_num  = ascii_hex(buf->data_num);

	for(i = 0, p = (PTR)&buf->data; i < rzi041s.data_num; i++, p++) {
		rzi041s.data[i].sta_flag = ascii_hex(p->sta_flag);
		rzi041s.data[i].weight   = htol((char *)&p->weight);
		rzi041s.data[i].count    = htol((char *)&p->count);
		rzi041s.data[i].ser_no   = htoc((char *)&p->ser_no);
	}
	return(0);
}
