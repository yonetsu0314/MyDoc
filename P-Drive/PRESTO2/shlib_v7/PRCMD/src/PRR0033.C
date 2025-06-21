/*======================================================================
 * File name	: prr0033.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0033.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0033.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr033.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr033.c
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
#include	<prcmd\prr0033.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_zero_adj						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	零点調整テキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_zero_adj(
	struct fmt_zero_adj	*buf
	)
{
	prr0033s.ch_id = ascii_hex(buf->ch_id);
	prr0033s.nof_head = htoc((char *)buf->nof_head);
	prr0033s.adj_head_ptn = htol((char *)buf->adj_head_ptn);
	prr0033s.adj_df_ptn = htoc((char *)buf->adj_df_ptn );
	prr0033s.init_zero_flag = ascii_hex(buf->init_zero_flag);
	prr0033s.ad_coef = htoi((char *)buf->ad_coef);
	prr0033s.da_coef = htoi((char *)buf->da_coef);
	prr0033s.range = htoi((char *)buf->range);
	return(0);
}
