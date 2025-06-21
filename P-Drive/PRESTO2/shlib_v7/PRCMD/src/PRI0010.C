/*======================================================================
 * File name	: pri0010.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi010a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi010a.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_prgno							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	プログラム番号構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prgno)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_prgno(
	struct fmt_prgno	*txt,
	struct pri0009t		*pkt
	)
{
	txt->brd_id = hex_ascii(pkt->brd_id);
	txt->dev_no = hex_ascii(pkt->dev_no);
	ctoh(pkt->node_id, (char *)txt->node_id);
	ctoh(pkt->len, (char *)txt->len);
	strncpy((char *)txt->rom_id, (char *)&pkt->rom_id, (size_t)pkt->len);
	return &txt->rom_id[pkt->len];
}
