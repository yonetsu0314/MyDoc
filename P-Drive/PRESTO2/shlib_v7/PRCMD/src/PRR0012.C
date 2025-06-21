/*======================================================================
 * File name	: prr0012.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0012.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0012.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr012.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr012.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0011.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_cbcal_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	組み合わせ計算特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cbcal_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_cbcal_spec(
	struct fmt_cbcal_spec	*txt,
	struct prr0011t		*pkt
	)
{
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	ltoh( pkt->wh_active, (char *)txt->wh_active );
	ltoh( pkt->bh_active, (char *)txt->bh_active );
	ctoh( pkt->compensation, (char *)txt->compensation );
	txt->renew_compensation = hex_ascii( pkt->renew_compensation );
	txt->calc_mode = hex_ascii( pkt->calc_mode );
	txt->mix_mode = hex_ascii( pkt->mix_mode );
	txt->over_scale = hex_ascii( pkt->over_scale );
	txt->over_weight = hex_ascii( pkt->over_weight );
	txt->hop_error = hex_ascii( pkt->hop_error );
	txt->az_error = hex_ascii( pkt->az_error );
	txt->stable = hex_ascii( pkt->stable );
	txt->re_check = hex_ascii( pkt->re_check );
	txt->th_error = hex_ascii( pkt->th_error );
	ctoh( pkt->nof_stop_head, (char *)txt->nof_stop_head );
	ctoh( pkt->nof_stop_cycle, (char *)txt->nof_stop_cycle );
	txt->err_rej_type = hex_ascii( pkt->err_rej_type );
	txt->multi_cal = hex_ascii( pkt->multi_cal );
	txt->approval_flag = hex_ascii( pkt->approval_flag );
	itoh( pkt->max_weight, (char *)txt->max_weight );
	itoh( pkt->min_weight, (char *)txt->min_weight );
	return(++txt);
}
