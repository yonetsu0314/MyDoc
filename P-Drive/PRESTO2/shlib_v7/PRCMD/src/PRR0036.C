/*======================================================================
 * File name	: prr0036.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	スパン調整テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0036.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0036.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr036.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr036.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0035.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_span_adj						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	スパン校正構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_span_adj)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_span_adj(
	struct fmt_span_adj	*txt,
	struct prr0035t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	ltoh(pkt->adj_head_ptn, (char *)txt->adj_head_ptn);
	ltoh(pkt->date, (char *)txt->date);
	ltoh(pkt->time, (char *)txt->time);
	return(++txt);
}
