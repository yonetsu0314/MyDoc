/*======================================================================
 * File name    : rzr012.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr012.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr012.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr012.cv  2.1  93/11/02 19:16:00  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr012.cv
 * Revision 2.1  93/11/02 19:16:00  ryoji
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
#include	<rzcmd\rzr011.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_cbcal_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算特性構造体データをテキストに変換する。		*/
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
rz_print_cbcal_spec(
	struct fmt_cbcal_spec	*txt,
	struct rzr011t		*pkt
	)
{
	ctoh( pkt->nof_head, (char *)&txt->nof_head );
	ltoh((unsigned long) pkt->wh_active, (char *)&txt->wh_active );
	ltoh((unsigned long) pkt->bh_active, (char *)&txt->bh_active );
	ctoh( pkt->compensation, (char *)&txt->compensation );
	txt->renew_compensation = hex_ascii( pkt->renew_compensation );
	txt->calc_mode 		= hex_ascii( pkt->calc_mode );
	txt->mix_mode     	= hex_ascii( pkt->mix_mode );
	txt->over_scale      	= hex_ascii( pkt->over_scale );
	txt->over_weight	= hex_ascii( pkt->over_weight );
	txt->hop_error 		= hex_ascii( pkt->hop_error );
	txt->az_error    	= hex_ascii( pkt->az_error );
	txt->err_rej_type      	= hex_ascii( pkt->err_rej_type );
	return(++txt);
}
