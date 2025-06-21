/*======================================================================
 * File name    : rzi112.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張秤データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi112.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi112.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi112.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzi112.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzi111.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ph_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張秤データ構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ph_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_ph_weight(
	struct fmt_ph_weight	*txt,
	struct rzi111t		*pkt
	)
{
	PR111	*buf;
	int	i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->min_ph_no, (char *)txt->min_ph_no);
	ctoh(pkt->nof_ph, (char *)txt->nof_ph);
	buf = txt->ph;
	for(i = pkt->nof_ph, j = pkt->min_ph_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->ph[j].weight, (char *)buf->weight);
		ctoh((unsigned char)pkt->ph[j].status, (char *)buf->status);
		buf++;
	}
	return(buf);
}
