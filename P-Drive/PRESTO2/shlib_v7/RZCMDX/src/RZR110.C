/*======================================================================
 * File name    : rzr110.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性テキスト作成（拡張１）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr110.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr110.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr110.c 1.2 1996/09/27 16:15:36 hiroki
 *Log: rzr110.c
 * リビジョン 1.2  1996/09/27  16:15:36  hiroki
 * weifht -> weight に修正。
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
#include	<rzcmd\rzr109.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_left_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤計量特性構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_left_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_left_weight(
	struct fmt_left_weight	*txt,
	struct rzr109t		*pkt
	)
{
	itoh( pkt->left_weight_cnt, (char *)&txt->left_weight_cnt);
	return(++txt);
}
