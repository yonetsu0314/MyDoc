/*======================================================================
 * File name    : pri0041.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（応答）テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0041.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0041.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi041.c 1.1 1970/01/01
 *Log: rzi041.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show prcmd		*/
#define		STORAGE
#include	<prcmd\pri0041.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_shift_data						*/
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
pr_scan_shift_data(
	struct fmt_shift_data	*buf
	)
{
	PI00410	*p;
	int	i;

	pri0041s.ch_id = ascii_hex(buf->ch_id);
	pri0041s.all_num   = htoc((char *)buf->all_num);
	pri0041s.data_page = htoc((char *)buf->data_page);
	pri0041s.data_num  = ascii_hex(buf->data_num);

	for(i = 0, p = (PTR)&buf->data; i < pri0041s.data_num; i++, p++) {
		pri0041s.data[i].sta_flag = ascii_hex(p->sta_flag);
		pri0041s.data[i].weight   = htol((char *)&p->weight);
		pri0041s.data[i].count    = htol((char *)&p->count);
		pri0041s.data[i].ser_no   = htoc((char *)&p->ser_no);
	}
	return(0);
}
