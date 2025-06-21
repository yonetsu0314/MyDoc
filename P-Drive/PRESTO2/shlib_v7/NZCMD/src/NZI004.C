/*======================================================================
 * File name    : nzi004.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi004.c 1.1 1999/05/27 16:51:15 kawamura
 *Log: nzi004.c
 * リビジョン 1.1  1999/05/27  16:51:15  kawamura
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
#include	<nzcmd\nzi003.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_cb_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_cb_result(
	struct fmt_cb_result_nz	*txt,
	struct nzi003t		*pkt
	)
{
	BYTE		*buf;
	NZI0030T	*p;
	int		i, j;
	char		str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		*buf = hex_ascii(p->flag);
		buf += sizeof(txt->sect[i].flag);
		ltoh((unsigned long)p->weight, (char *)buf);
		buf += sizeof(txt->sect[i].weight);
		itoh((unsigned short)p->count, (char *)buf);
		buf += sizeof(txt->sect[i].count);
	}
	*buf = hex_ascii(pkt->ch.flag);
	buf += sizeof(txt->ch.flag);
	ltoh((unsigned long)pkt->ch.weight, (char *)buf);
	buf += sizeof(txt->ch.weight);
	itoh((unsigned short)pkt->ch.count, (char *)buf);
	buf += sizeof(txt->ch.count);
	ltoh((unsigned long)pkt->slv_targ_wt, (char *)buf);
	buf += sizeof(txt->slv_targ_wt);
	itoh((unsigned short)pkt->slv_targ_cnt, (char *)buf);
	buf += sizeof(txt->slv_targ_cnt);
	ctoh((unsigned char)pkt->min_wh_no, (char *)buf);
	buf += sizeof(txt->min_wh_no);
	ctoh((unsigned char)pkt->nof_wh, (char *)buf);
	buf += sizeof(txt->nof_wh);
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh_status[j], (char *)str);
		memcpy((void *)buf, (void *)&str[1], sizeof(txt->wh_status[0]));
		buf += sizeof(txt->wh_status[0]);
	}
	ctoh((unsigned char)pkt->nof_bh, (char *)buf);
	buf += sizeof(txt->nof_bh);
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->bh_status[j], (char *)str);
		memcpy((void *)buf, (void *)&str[1], sizeof(txt->bh_status[0]));
		buf += sizeof(txt->bh_status[0]);
	}
	return(buf);
}
