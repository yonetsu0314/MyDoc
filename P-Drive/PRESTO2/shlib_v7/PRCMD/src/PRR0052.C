/*======================================================================
 * File name	: prr0052.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0052.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0052.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr052.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr052.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0051.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	テスト駆動停止構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tstdrv_run)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_tstdrv_run(
	struct fmt_tstdrv_run	*txt,
	struct prr0051t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	ctoh(pkt->cf_ptn, (char *)txt->cf_ptn);
	ctoh(pkt->df_ptn, (char *)txt->df_ptn);
	ltoh(pkt->af_ptn, (char *)txt->af_ptn);
	ltoh(pkt->rf_ptn, (char *)txt->rf_ptn);
	ltoh(pkt->sht_ptn, (char *)txt->sht_ptn);
	ltoh(pkt->ph_ptn, (char *)txt->ph_ptn);
	ltoh(pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh(pkt->wh2_ptn, (char *)txt->wh2_ptn);
	ltoh(pkt->bh_ptn, (char *)txt->bh_ptn);
	itoh(pkt->rs_ptn, (char *)txt->rs_ptn);
	itoh(pkt->dth_ptn, (char *)txt->dth_ptn);
	itoh(pkt->th_ptn, (char *)txt->th_ptn);
	ltoh(pkt->spa_ptn, (char *)txt->spa_ptn);
	txt->drv_cyc = hex_ascii( pkt->drv_cyc );
	txt->sht_flg = hex_ascii( pkt->sht_flg );
	txt->spa_flg = hex_ascii( pkt->spa_flg );
	return(++txt);
}
