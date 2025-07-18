/*======================================================================
 * File name	: prr0056.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ調整テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0056.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0056.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzr204.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzr204.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0055.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_afv_adj						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤診断データ要求の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_afv_adj)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_afv_adj(
	struct fmt_afv_adj	*txt,
	struct prr0055t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	itoh((unsigned short)pkt->adj_mode, (char *)txt->adj_mode);
	return(++txt);
}
