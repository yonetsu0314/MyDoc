/*======================================================================
 * File name    : rzi240.c
 * Original	: rzi140.c
 *----------------------------------------------------------------------
 * Function	:
 * ＡＴＡ98モニター情報テキスト作成（拡張２）ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi240.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi240.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi240.c 1.2 1999/04/06 12:11:09 hiroki
 *Log: rzi240.c
 * リビジョン 1.2  1999/04/06  12:11:09  hiroki
 * SDEenable -> SDEnable 修正
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
#include	<rzcmd\rzi239.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_stale98_time						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ステイル・ロックタイム・テキストに変換する。			*/
/*	（ＡＴＡ９８仕様）						*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ata_monitor)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_stale98_time(
	struct fmt_stale98_time	*txt,
	struct rzi239t		*pkt
	)

{
	itoh( (unsigned short)pkt->stale98_time, (char *)&txt->stale98_time );
	txt->stale98_mode = hex_ascii( pkt->stale98_mode );
	itoh( (unsigned short)pkt->stale98_tmr, (char *)&txt->stale98_tmr );
	ctoh( pkt->stale98_dcnt, (char *)txt->stale98_dcnt);
	txt->stale98_SDEnable = hex_ascii( pkt->stale98_SDEnable );
	txt->stale98_drain_mode = hex_ascii( pkt->stale98_drain_mode );
	itoh( (unsigned short)pkt->stale98_drain_tmr, (char *)&txt->stale98_drain_tmr );

	return(++txt);
}
