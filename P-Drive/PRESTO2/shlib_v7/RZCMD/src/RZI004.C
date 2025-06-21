/*======================================================================
 * File name    : rzi004.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi004.cv  2.1  93/11/02 19:10:22  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi004.cv
 * Revision 2.1  93/11/02 19:10:22  ryoji
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
#include	<rzcmd\rzi003.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_cb_result						*/
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
rz_print_cb_result(
	struct fmt_cb_result	*txt,
	struct rzi003t		*pkt
	)
{
	BYTE		*buf;
	RZI0030T	*p;
	int		i, j;

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
	ctoh(pkt->min_wh_no, (char *)buf);
	buf += sizeof(txt->min_wh_no);
	ctoh(pkt->nof_wh, (char *)buf);
	buf += sizeof(txt->nof_wh);
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		ctoh((unsigned char)pkt->wh_status[j], (char *)buf);
		buf += sizeof(txt->wh_status[0]);
	}
	ctoh(pkt->nof_bh, (char *)buf);
	buf += sizeof(txt->nof_bh);
	for(i = pkt->nof_bh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		ctoh((unsigned char)pkt->bh_status[j], (char *)buf);
		buf += sizeof(txt->bh_status[0]);
	}
	return(buf);
}
