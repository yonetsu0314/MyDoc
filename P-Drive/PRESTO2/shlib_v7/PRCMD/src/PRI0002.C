/*======================================================================
 * File name	: pri0002.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0002.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0002.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi002.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi002.c
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
#include	<custom\xmemory.h>		/**/
#include	<prcmd\pri0001.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_diag_data						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤診断データをテキストに変換する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_diag_data)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_diag_data(
	struct fmt_diag_data	*txt,
	struct pri0001t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;
	char	str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh(pkt->nof_wh, (char *)txt->nof_wh);
	buf = (BYTE_PTR)txt->wh_3rd;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_3rd[j].weight, (char *)buf);
		buf += sizeof(txt->wh_3rd[j].weight);
		itoh((unsigned short)pkt->wh_3rd[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->wh_3rd[j].status));
		buf += sizeof(txt->wh_3rd[j].status);
	}
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_4th[j].weight, (char *)buf);
		buf += sizeof(txt->wh_4th[j].weight);
		itoh((unsigned short)pkt->wh_4th[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->wh_4th[j].status));
		buf += sizeof(txt->wh_4th[j].status);
	}
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_5th[j].weight, (char *)buf);
		buf += sizeof(txt->wh_5th[j].weight);
		itoh((unsigned short)pkt->wh_5th[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->wh_5th[j].status));
		buf += sizeof(txt->wh_5th[j].status);
	}
	for(i = pkt->nof_ph, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->ph_3rd[j].weight, (char *)buf);
		buf += sizeof(txt->ph_3rd[j].weight);
		itoh((unsigned short)pkt->ph_3rd[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->ph_3rd[j].status));
		buf += sizeof(txt->ph_3rd[j].status);
	}
	for(i = pkt->nof_ph, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->ph_4th[j].weight, (char *)buf);
		buf += sizeof(txt->ph_4th[j].weight);
		itoh((unsigned short)pkt->ph_4th[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->ph_4th[j].status));
		buf += sizeof(txt->ph_4th[j].status);
	}
	for(i = pkt->nof_ph, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->ph_5th[j].weight, (char *)buf);
		buf += sizeof(txt->ph_5th[j].weight);
		itoh((unsigned short)pkt->ph_5th[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->ph_5th[j].status));
		buf += sizeof(txt->ph_5th[j].status);
	}
	*buf = hex_ascii( pkt->min_df_no );
	buf += sizeof(txt->min_df_no);
	*buf = hex_ascii( pkt->nof_df );
	buf += sizeof(txt->nof_df);
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->df[j].weight, (char *)buf);
		buf += sizeof(txt->df[j].weight);
		itoh((unsigned short)pkt->df[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->df[j].status));
		buf += sizeof(txt->df[j].status);
	}
	ctoh(pkt->nof_afv, (char *)buf);
	buf += sizeof(txt->nof_afv);
	for(i = 0 ; i < pkt->nof_afv ; i++ ) {
		itoh(pkt->afv_out[i], (char *)buf );
		buf += sizeof(txt->afv_out[i]);
	}
	return(buf);
}
