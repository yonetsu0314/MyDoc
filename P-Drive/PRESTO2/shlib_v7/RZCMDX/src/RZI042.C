/*======================================================================
 * File name    : rzi042.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（応答）テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi042.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi042.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi042.c 1.3 1996/12/19 20:29:19 hiroki
 *Log: rzi042.c
 * リビジョン 1.3  1996/12/19  20:29:19  hiroki
 * itoh を ctoh に修正する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show rzcmd		*/
#include	<rzcmd\rzi041.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_shift_data						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	シフトデータテキスト（応答）をテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_shift_data)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_shift_data(
	struct fmt_shift_data	*txt,
	struct rzi041t		*pkt
	)
{

	BYTE		*buf;
	RZI0410T	*p;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->all_num, (char *)txt->all_num);
	ctoh(pkt->data_page, (char *)txt->data_page);
	txt->data_num = hex_ascii(pkt->data_num);

	/*ここから可変長データ処理*/
	buf = (BYTE_PTR)txt->data;
	for(i = 0, p = (PTR)&pkt->data; i < pkt->data_num ; i++, p++) {
		*buf = hex_ascii(p->sta_flag);
		buf += sizeof(txt->data[i].sta_flag);
		ltoh((unsigned long)p->weight, (char *)buf);
		buf += sizeof(txt->data[i].weight);
		ltoh((unsigned long)p->count, (char *)buf);
		buf += sizeof(txt->data[i].count);
		ctoh(p->ser_no, (char *)buf);
		buf += sizeof(txt->data[i].ser_no);
	}
	return(buf);
}
