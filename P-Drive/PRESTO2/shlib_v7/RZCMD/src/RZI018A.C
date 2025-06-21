/*======================================================================
 * File name    : rzi018a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用異常発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi018a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi018a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi018a.c 1.1 1995/11/21 09:23:39 hiroe
 *Log: rzi018a.c
 * リビジョン 1.1  1995/11/21  09:23:39  hiroe
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
#include	<rzcmd\rzi017a.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_error_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用エラー構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_error_v3)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_error_v3(
	struct fmt_error_v3	*txt,
	struct rzi017t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	ctoh((unsigned char)pkt->error, (char *)txt->error);
	ltoh((unsigned long)pkt->ph_ptn, (char *)txt->ph_ptn);
	ltoh((unsigned long)pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh((unsigned long)pkt->bh_ptn, (char *)txt->bh_ptn);
	ctoh(pkt->rs_ptn, (char *)txt->rs_ptn);
	ctoh(pkt->dth_ptn, (char *)txt->dth_ptn);
	ctoh(pkt->th_ptn, (char *)txt->th_ptn);
	ctoh(pkt->df_ptn, (char *)txt->df_ptn);
	txt->duc_inf = hex_ascii(pkt->duc_inf);
	txt->exc_inf = hex_ascii(pkt->exc_inf);
	return ++txt;
}
