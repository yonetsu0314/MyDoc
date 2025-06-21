/*======================================================================
 * File name    : nzi002.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi002.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: nzi002.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  15:24
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
 *Header: p:/lib/nzcmd/src/rcs/nzi002.c 1.1 1999/05/27 16:51:24 kawamura
 *Log: nzi002.c
 * リビジョン 1.1  1999/05/27  16:51:24  kawamura
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
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<custom\xmemory.h>		/**/
#include	<nzcmd\nzi001.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_diag_data						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤診断データをテキストに変換する。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_diag_data(
	struct fmt_diag_data_nz	*txt,
	struct nzi001t		*pkt
	)
{
	BYTE		*buf1;
	NI0010		*buf;
	int		i, j;
	char		str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh((unsigned char)pkt->nof_wh, (char *)txt->nof_wh);
	buf = txt->wh_3rd;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_3rd[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->wh_3rd[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_4th[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->wh_4th[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	*(BYTE_PTR)buf = hex_ascii( pkt->min_df_no );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->min_df_no);
	buf = (NI0010 *)buf1;
	*(BYTE_PTR)buf = hex_ascii( pkt->nof_df );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_df);
	buf = (NI0010 *)buf1;
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->df[j].weight, (char *)buf->weight);
		itoh((unsigned short)pkt->df[j].status, (char *)str);
		memcpy((void *)buf->status, (void *)&str[1], sizeof(buf->status));
		buf++;
	}
	buf1 = (PTR)buf;
	*buf1 = hex_ascii( pkt->nof_afv );
	buf1 += sizeof(txt->nof_afv);
	for(j = 0 ; j< pkt->nof_afv ; j++) {
		itoh((unsigned short)pkt->afv_out[j], (char *)buf1 );
		buf1 += sizeof(txt->afv_out[0]);
	}
	return(buf1);
}
