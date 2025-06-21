/*======================================================================
 * File name    : nzi012.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi012.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: nzi012.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  15:13
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:07
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi012.c 1.1 1999/05/27 16:50:58 kawamura
 *Log: nzi012.c
 * リビジョン 1.1  1999/05/27  16:50:58  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/**/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<nzcmd\nzi011.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_head_weight						*/
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
nz_print_head_weight(
	struct fmt_head_weight_nz	*txt,
	struct nzi011t			*pkt
	)
{
	N9	*buf;
	BYTE	*buf1;
	int	i, j;
	char	str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh((unsigned char)pkt->nof_wh, (char *)txt->nof_wh);
	buf = txt->wh;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->wh[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	ctoh((unsigned char)pkt->nof_bh, (char *)buf);
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_bh);
	buf = (N9 *)buf1;
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->bh[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->bh[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	*(BYTE_PTR)buf = hex_ascii( pkt->min_df_no );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->min_df_no);
	buf = (N9 *)buf1;
	*(BYTE_PTR)buf = hex_ascii( pkt->nof_df );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_df);
	buf = (N9 *)buf1;
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->df[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->df[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	return(buf);
}
