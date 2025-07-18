/*======================================================================
 * File name    : prr0028.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤データ要求テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0028.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0028.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzr028.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr028.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0027.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_req_weidat						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤データ要求の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_req_weidat)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_req_weidat(
	struct fmt_req_weidat	*txt,
	struct prr0027t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	return(++txt);
}
