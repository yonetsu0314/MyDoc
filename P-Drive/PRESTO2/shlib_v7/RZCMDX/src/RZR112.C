/*======================================================================
 * File name    : rzr112.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr112.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr112.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr112.cv  1.1  94/06/22 17:34:04  hiroe
 *Log: p:/lib/rzcmdx/src/rcs/rzr112.cv
 * Revision 1.1  94/06/22 17:34:04  hiroe
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
#include	<rzcmd\rzr111.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_approval						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	検定に必要な構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cbcal_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_approval(
	struct fmt_approval	*txt,
	struct rzr111t		*pkt
	)
{
	txt->approval_flag 	= hex_ascii( pkt->approval_flag );
	txt->vol_flag		= hex_ascii( pkt->vol_flag );
	itoh( pkt->max_weight, (char *)&txt->max_weight );
	itoh( pkt->min_weight, (char *)&txt->min_weight );
	return(++txt);
}
