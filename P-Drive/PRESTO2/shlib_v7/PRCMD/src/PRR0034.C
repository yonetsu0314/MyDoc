/*======================================================================
 * File name	: prr0034.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0034.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0034.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr034.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr034.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0033.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_zero_adj						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	零点調整構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_zero_adj)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_zero_adj(
	struct fmt_zero_adj	*txt,
	struct prr0033t		*pkt
	)
{
	txt->ch_id = hex_ascii( pkt->ch_id );
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	ltoh(pkt->adj_head_ptn, (char *)txt->adj_head_ptn );
	ctoh( pkt->adj_df_ptn, (char *)txt->adj_df_ptn );
	txt->init_zero_flag = hex_ascii( pkt->init_zero_flag );
	itoh( pkt->ad_coef, (char *)txt->ad_coef );
	itoh( pkt->da_coef, (char *)txt->da_coef );
	itoh( pkt->range, (char *)txt->range );
	return(++txt);
}
