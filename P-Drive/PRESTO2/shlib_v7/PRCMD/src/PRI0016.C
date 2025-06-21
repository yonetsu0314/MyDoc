/*======================================================================
 * File name	: pri0016.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	警報発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0016.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0016.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi016.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi016.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0015.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_alarm							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	アラーム構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_alarm)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
pr_print_alarm(
	struct fmt_alarm	*txt,
	struct pri0015t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->alarm, (char *)txt->alarm);
	ltoh(pkt->ph_ptn, (char *)txt->ph_ptn);
	ltoh(pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh(pkt->bh_ptn, (char *)txt->bh_ptn);
	itoh(pkt->rs_ptn, (char *)txt->rs_ptn);
	itoh(pkt->dth_ptn, (char *)txt->dth_ptn);
	itoh(pkt->th_ptn, (char *)txt->th_ptn);
	itoh(pkt->df_ptn, (char *)txt->df_ptn);
	ltoh(pkt->af_ptn, (char *)txt->af_ptn);
	ctoh(pkt->afd, (char *)txt->afd);
	return(++txt);
}
