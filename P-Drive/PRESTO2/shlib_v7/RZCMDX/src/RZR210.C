/*======================================================================
 * File name    : rzr210.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張計量特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr210.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr210.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr210.c 1.2 1970/01/01 00:00:00 itami
 *Log: rzr210.c
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 * 2000/12/12 自動零点間隔追加
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
#include	<rzcmd\rzr209.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ph_wei_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張秤計量特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_ph_wei_spec(
	struct fmt_ph_wei_spec	*txt,
	struct rzr209t		*pkt
	)
{
	itoh( pkt->stable_time, (char *)&txt->stable_time );
	itoh( pkt->empty_weight, (char *)&txt->empty_weight );
	ctoh( pkt->zero_interval, (char *)&txt->zero_interval );
	return(++txt);
}
