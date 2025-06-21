/*======================================================================
 * File name    : rzi040.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター情報テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi040.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi040.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi040.c 1.3 1996/07/12 19:52:12 hiroki
 *Log: rzi040.c
 * リビジョン 1.3  1996/07/12  19:52:12  hiroki
 * ATA96仕様
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
#include	<rzcmd\rzi039.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＴＡモニター情報をテキストに変換する。			*/
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
rz_print_ata_monitor(
	struct fmt_ata_monitor	*txt,
	struct rzi039t		*pkt
	)
{
	txt->ch_id    = hex_ascii( pkt->ch_id );
	txt->ata_enable = hex_ascii( pkt->ata_enable );
	itoh( (unsigned short)pkt->ata_timer, (char *)&txt->ata_timer );
	txt->ata_mode = hex_ascii( pkt->ata_mode );
	ltoh( pkt->ata_tgt, (char *)&txt->ata_tgt );
	itoh( (unsigned short)pkt->ata_def_time, (char *)&txt->ata_def_time );
	itoh( (unsigned short)pkt->ata_cwb, (char *)&txt->ata_cwb );
	return(++txt);
}
