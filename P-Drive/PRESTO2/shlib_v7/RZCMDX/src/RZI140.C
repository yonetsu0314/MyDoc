/*======================================================================
 * File name    : rzi140.c
 * Original	: rzi040.c
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター情報テキスト作成（拡張１）ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi140.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi140.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi140.c 1.1 1998/04/01 17:42:45 hiroki
 *Log: rzi140.c
 * リビジョン 1.1  1998/04/01  17:42:45  hiroki
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
#include	<rzcmd\rzi139.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ステイル・ロックタイム・テキストに変換する。			*/
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
rz_print_stale_time(
	struct fmt_stale_time	*txt,
	struct rzi139t		*pkt
	)

{
	itoh( (unsigned short)pkt->stale_time, (char *)&txt->stale_time );
	txt->stale_mode = hex_ascii( pkt->stale_mode );
	itoh( (unsigned short)pkt->stale_tmr, (char *)&txt->stale_tmr );
	ctoh( pkt->stale_dcnt, (char *)txt->stale_dcnt);

	return(++txt);
}
