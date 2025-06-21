/*======================================================================
 * File name	: pri0054.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0054.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0054.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi008.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi008.c
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
#include	<prcmd\pri0053.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_archive						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	重量波形データ応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_archive)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_archive(
	struct fmt_archive	*txt,
	struct pri0053t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->hopper_id = hex_ascii( pkt->hopper_id );
	ctoh(pkt->wh_no, (char *)txt->wh_no);
	itoh(pkt->index, (char *)txt->index);
	ctoh(pkt->nof_data, (char *)txt->nof_data);
	buf = (BYTE_PTR)txt->data;
	for(i = 0 ; i < pkt->nof_data; i++) {
		itoh((unsigned short)pkt->data[i].weight, (char *)buf);
		buf += sizeof(txt->data[i].weight);
		*buf = hex_ascii( pkt->data[i].status );
		buf += sizeof(txt->data[i].status);
	}
	return(buf);
}
