/*======================================================================
 * File name	: pri0040.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター情報テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0040.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0040.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi040.c 1.3 1996/07/12 19:52:12 hiroki Exp
 *log: rzi040.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0039.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＴＡモニター情報をテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ata_monitor)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ata_monitor(
	struct fmt_ata_monitor	*txt,
	struct pri0039t		*pkt
	)
{
	txt->ch_id    = hex_ascii( pkt->ch_id );
	txt->ata_enable = hex_ascii( pkt->ata_enable );
	itoh( pkt->ata_timer, (char *)txt->ata_timer );
	txt->ata_mode = hex_ascii( pkt->ata_mode );
	ltoh( pkt->ata_tgt, (char *)txt->ata_tgt );
	itoh( pkt->ata_def_time, (char *)txt->ata_def_time );
	itoh( pkt->ata_cwb, (char *)txt->ata_cwb );
	return(++txt);
}
