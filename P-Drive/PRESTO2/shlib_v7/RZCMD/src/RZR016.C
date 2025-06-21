/*======================================================================
 * File name    : rzr016.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 フィーダ駆動特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr016.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr016.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr016.cv  2.1  93/11/02 19:18:54  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr016.cv
 * Revision 2.1  93/11/02 19:18:54  ryoji
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
#include	<rzcmd\rzr015.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_fd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ駆動特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_fd_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_fd_spec(
	struct fmt_fd_spec	*txt,
	struct rzr015t		*pkt
	)
{
	ctoh( pkt->nof_head, (char *)&txt->nof_head );
	ltoh((unsigned long) pkt->fd_assign_ptn, (char *)&txt->fd_assign_ptn );
	txt->feeder_id    = hex_ascii( pkt->feeder_id );
	txt->min_amp      = hex_ascii( pkt->min_amp );
	txt->max_amp      = hex_ascii( pkt->max_amp );
	txt->multiplier   = hex_ascii( pkt->multiplier );
	txt->phase_type   = hex_ascii( pkt->phase_type );
	txt->opt_sw_ctrl  = hex_ascii( pkt->opt_sw_ctrl );
	txt->shutter_ctrl = hex_ascii( pkt->shutter_ctrl );
	ctoh( pkt->shutter_off_delay, (char *)&txt->shutter_off_delay );
	return(++txt);
}
