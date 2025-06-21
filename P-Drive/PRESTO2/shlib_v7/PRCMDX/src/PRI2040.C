/*======================================================================
 * File name	: pri2040.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 * ＡＴＡ98モニター情報テキスト作成  ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri2040.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri2040.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi240.c 1.2 1999/04/06 12:11:09 hiroki Exp
 *log: rzi240.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri2039.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_stale98_time						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ステイル・ロックタイム・テキストに変換する。			*/
/*	（ＡＴＡ９８仕様）						*/
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
pr_print_stale98_time(
	struct fmt_stale98_time	*txt,
	struct pri2039t		*pkt
	)

{
	itoh( pkt->stale98_time, (char *)txt->stale98_time );
	txt->stale98_mode = hex_ascii( pkt->stale98_mode );
	itoh( pkt->stale98_tmr, (char *)txt->stale98_tmr );
	ctoh( pkt->stale98_dcnt, (char *)txt->stale98_dcnt);
	txt->stale98_SDEnable = hex_ascii( pkt->stale98_SDEnable );
	txt->stale98_drain_mode = hex_ascii( pkt->stale98_drain_mode );
	itoh( pkt->stale98_drain_tmr, (char *)txt->stale98_drain_tmr );
	return(++txt);
}
