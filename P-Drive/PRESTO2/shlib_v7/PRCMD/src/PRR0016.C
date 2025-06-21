/*======================================================================
 * File name	: prr0016.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	 フィーダ駆動特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0016.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0016.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr016.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr016.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0015.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_fd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_fd_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_fd_spec(
	struct fmt_fd_spec	*txt,
	struct prr0015t		*pkt
	)
{
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	ltoh( pkt->fd_assign_ptn, (char *)txt->fd_assign_ptn );
	txt->feeder_id = hex_ascii( pkt->feeder_id );
	txt->min_amp = hex_ascii( pkt->min_amp );
	txt->max_amp = hex_ascii( pkt->max_amp );
	txt->multiplier = hex_ascii( pkt->multiplier );
	txt->phase_type = hex_ascii( pkt->phase_type );
	txt->opt_sw_ctrl = hex_ascii( pkt->opt_sw_ctrl );
	txt->shutter_ctrl = hex_ascii( pkt->shutter_ctrl );
	ctoh( pkt->shutter_off_delay, (char *)txt->shutter_off_delay );
	txt->slow_start = hex_ascii( pkt->slow_start );
	txt->stop_ptn = hex_ascii( pkt->stop_ptn );
	return(++txt);
}
