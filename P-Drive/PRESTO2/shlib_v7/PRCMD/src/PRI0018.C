/*======================================================================
 * File name	: pri0018.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0018.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi018a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi018a.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_error							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	エラー構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_error)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_error(
	struct fmt_error	*txt,
	struct pri0017t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->error, (char *)txt->error);
	ltoh(pkt->ph_ptn, (char *)txt->ph_ptn);
	ltoh(pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh(pkt->bh_ptn, (char *)txt->bh_ptn);
	itoh(pkt->rs_ptn, (char *)txt->rs_ptn);
	itoh(pkt->dth_ptn, (char *)txt->dth_ptn);
	itoh(pkt->th_ptn, (char *)txt->th_ptn);
	ctoh(pkt->df_ptn, (char *)txt->df_ptn);
	ltoh(pkt->af_ptn, (char *)txt->af_ptn);
	txt->duc_inf = hex_ascii(pkt->duc_inf);
	txt->exc_inf = hex_ascii(pkt->exc_inf);
	return ++txt;
}
