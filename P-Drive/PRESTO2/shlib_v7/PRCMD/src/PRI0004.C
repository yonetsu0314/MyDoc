/*======================================================================
 * File name	: pri0004.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi004.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi004.c
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
#include	<prcmd\pri0003.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_cb_result						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	組み合わせ計算結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_cb_result(
	struct fmt_cb_result	*txt,
	struct pri0003t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;
	char	str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i = 0 ; i < pkt->nof_sect ; i++ ) {
		ctoh( (unsigned char)pkt->sect[i].flag, (char *)buf );
		buf += sizeof( txt->sect[i].flag );
		ltoh((unsigned long)pkt->sect[i].weight, (char *)buf);
		buf += sizeof(txt->sect[i].weight);
		itoh((unsigned short)pkt->sect[i].count, (char *)buf);
		buf += sizeof(txt->sect[i].count);
	}
	ctoh( (unsigned char)pkt->ch.flag, (char *)buf );
	buf += sizeof(txt->ch.flag);
	ltoh((unsigned long)pkt->ch.weight, (char *)buf);
	buf += sizeof(txt->ch.weight);
	itoh((unsigned short)pkt->ch.count, (char *)buf);
	buf += sizeof(txt->ch.count);
	ctoh( pkt->multi_dump_seq_no, (char *)buf );
	buf += sizeof(txt->multi_dump_seq_no);
	ltoh((unsigned long)pkt->slv_targ_wt, (char *)buf);
	buf += sizeof(txt->slv_targ_wt);
	itoh((unsigned short)pkt->slv_targ_cnt, (char *)buf);
	buf += sizeof(txt->slv_targ_cnt);
	ctoh( pkt->min_wh_no, (char *)buf);
	buf += sizeof(txt->min_wh_no);
	ctoh( pkt->nof_wh, (char *)buf);
	buf += sizeof(txt->nof_wh);
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_status[j], (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->wh_status[j]));
		buf += sizeof(txt->wh_status[j]);
	}
	ctoh((unsigned char)pkt->nof_bh, (char *)buf);
	buf += sizeof(txt->nof_bh);
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->bh_status[j], (char *)str);
		memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->bh_status[j]));
		buf += sizeof(txt->bh_status[j]);
	}
	return(buf);
}
