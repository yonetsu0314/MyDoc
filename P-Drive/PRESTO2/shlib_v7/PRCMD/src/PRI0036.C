/*======================================================================
 * File name	: pri0036.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0036.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0036.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi036.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi036.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show prcmd		*/
#include	<prcmd\pri0035.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_netmap							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ネットワーク参加状態構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_netmap)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_netmap(
	struct fmt_netmap	*txt,
	struct pri0035t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->brd_id = hex_ascii(pkt->brd_id);
	ctoh(pkt->dev_no, (char *)txt->dev_no);
	buf = (BYTE_PTR)txt->net_map;
	for( i = 0 ; i < sizeof(pkt->net_map) ; i++ ) {
		ctoh( pkt->net_map[i], (char *)buf );
		buf += sizeof( txt->net_map[i] );
	}
	itoh(pkt->recon, (char *)txt->recon);
	return(++txt);
}
