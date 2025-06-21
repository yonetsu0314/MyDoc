/*======================================================================
 * File name    : rzi012.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi012.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzi012.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:44
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:48
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi012.cv  2.1  93/11/02 19:08:22  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi012.cv
 * Revision 2.1  93/11/02 19:08:22  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzi011.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_head_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤データ構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_head_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_head_weight(
	struct fmt_head_weight	*txt,
	struct rzi011t		*pkt
	)
{
	P9		*buf;
	BYTE		*buf1;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh(pkt->nof_wh, (char *)txt->nof_wh);
	buf = txt->wh;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->wh[j].status, (char *)buf->status);
		buf++;
	}
	ctoh(pkt->nof_bh, (char *)buf);
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_bh);
	buf = (P9 *)buf1;
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->bh[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->bh[j].status, (char *)buf->status);
		buf++;
	}
	*(BYTE_PTR)buf = hex_ascii( pkt->min_df_no );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->min_df_no);
	buf = (P9 *)buf1;
	*(BYTE_PTR)buf = hex_ascii( pkt->nof_df );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_df);
	buf = (P9 *)buf1;
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->df[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->df[j].status, (char *)buf->status);
		buf++;
	}
	return(buf);
}
