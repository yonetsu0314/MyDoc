/*======================================================================
 * File name	: prr0035.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	スパン調整テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0035.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0035.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr035.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr035.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0035.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_span_adj						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	スパン校正テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_span_adj(
	struct fmt_span_adj	*buf
	)
{
	prr0035s.ch_id = ascii_hex(buf->ch_id);
	prr0035s.nof_head = htoc((char *)buf->nof_head);
	prr0035s.adj_head_ptn = htol((char *)buf->adj_head_ptn);
	prr0035s.date = htol((char *)buf->date);
	prr0035s.time = htol((char *)buf->time);
	return(0);
}
