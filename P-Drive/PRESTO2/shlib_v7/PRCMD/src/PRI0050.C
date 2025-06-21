/*======================================================================
 * File name	: pri0050.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ロジック波形データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0050.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0050.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi006.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi006.c
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
#include	<prcmd\pri0049.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_logic							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ロジック波形データ応答構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_logic)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_logic(
	struct fmt_logic	*txt,
	struct pri0049t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->unit_no = hex_ascii( pkt->unit_no );
	ctoh(pkt->nof_data, (char *)txt->nof_data);
	buf = (BYTE_PTR)txt->data;
	for(i = 0 ; i < pkt->nof_data; i++) {
		itoh(pkt->data[i], (char *)buf);
		buf += sizeof(txt->data[i]);
	}
	return(buf);
}
