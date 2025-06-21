/*======================================================================
 * File name	: pri0006.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	単重テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi006.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi006.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0005.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_piece_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	単重自動更新結果の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_piece_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_piece_weight(
	struct fmt_piece_weight	*txt,
	struct pri0005t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->weight;
	for(i = 0; i < pkt->nof_sect ; i++) {
		itoh(pkt->weight[i], (char *)buf);
		buf += sizeof(txt->weight[i]);
	}
	return(buf);
}
