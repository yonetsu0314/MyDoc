/*======================================================================
 * File name    : dzi206.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzi206.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: dzi206.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  13:50
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:18
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzi206.cv  1.6  94/03/23 16:56:20  kabumoto
 *Log: P:/LIB/DZCMD/SRC/RCS/dzi206.cv
 * Revision 1.6  94/03/23 16:56:20  kabumoto
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#include	<dzcmd\dzi205.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	dz_print_diag_data						*/
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
dz_print_diag_data(
	struct fmt_diag_data	*txt,
	struct dzi205t		*pkt
	)
{
	BYTE		*buf1;
	PI2050		*buf;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh((unsigned char)pkt->nof_wh, (char *)txt->nof_wh);
	buf = txt->wh_3rd;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_3rd[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->wh_3rd[j].status, (char *)buf->status);
		buf++;
	}
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_4th[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->wh_4th[j].status, (char *)buf->status);
		buf++;
	}
	*(BYTE_PTR)buf = hex_ascii( pkt->min_df_no );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->min_df_no);
	buf = (PI2050 *)buf1;
	*(BYTE_PTR)buf = hex_ascii( pkt->nof_df );
	buf1 = (BYTE_PTR)buf;
	buf1 += sizeof(txt->nof_df);
	buf = (PI2050 *)buf1;
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->df[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->df[j].status, (char *)buf->status);
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
