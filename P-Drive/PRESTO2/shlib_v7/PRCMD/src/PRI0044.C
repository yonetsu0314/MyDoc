/*======================================================================
 * File name	: pri0044.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0044.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0044.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmdx/src/rcs/rzi044.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi044.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0043.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_span_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	スパン係数応答構造体のデータをテキストに変換する。		*/
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
pr_print_span_coeffi(
	struct fmt_span_coeffi	*txt,
	struct pri0043t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->pwb_id = hex_ascii( pkt->pwb_id );
	txt->range = hex_ascii( pkt->range );
	ltoh(pkt->date, (char *)txt->date);
	ltoh(pkt->time, (char *)txt->time);
	buf = (BYTE_PTR)txt->span_coeffi[0];
	for(i= 0; i< sizeof(pkt->span_coeffi)/sizeof(pkt->span_coeffi[0]); i++) {
		itoh( pkt->span_coeffi[i], (char *)buf );
		buf += sizeof( txt->span_coeffi[i] );
	}
	return(buf);
}
