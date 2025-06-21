/*======================================================================
 * File name	: pri0012.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0012.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0012.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi012.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi012.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/**/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0011.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_head_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤データ構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_head_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_head_weight(
	struct fmt_head_weight	*txt,
	struct pri0011t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;
	char	str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->flag = hex_ascii(pkt->flag);
	ctoh(pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh(pkt->nof_wh, (char *)txt->nof_wh);
	buf = (BYTE_PTR)txt->wh;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh[j].weight, (char *)buf);
		buf += sizeof(txt->wh[j].weight);
		itoh((unsigned short)pkt->wh[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->wh[j].status));
		buf += sizeof(txt->wh[j].status);
	}
	ctoh(pkt->nof_bh, (char *)buf);
	buf += sizeof(txt->nof_bh);
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->bh[j].weight, (char *)buf);
		buf += sizeof(txt->bh[j].weight);
		itoh((unsigned short)pkt->bh[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->bh[j].status));
		buf += sizeof(txt->bh[j].status);
	}
	ctoh(pkt->nof_ph, (char *)buf);
	buf += sizeof(txt->nof_ph);
	for(i = pkt->nof_ph, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->ph[j].weight, (char *)buf);
		buf += sizeof(txt->ph[j].weight);
		itoh((unsigned short)pkt->ph[j].status, (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->ph[j].status));
		buf += sizeof(txt->ph[j].status);
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
	return(buf);
}
