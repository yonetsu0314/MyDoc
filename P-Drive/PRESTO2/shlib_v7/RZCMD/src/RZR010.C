/*======================================================================
 * File name    : rzr010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr010.cv  2.1  93/11/02 19:16:22  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr010.cv
 * Revision 2.1  93/11/02 19:16:22  ryoji
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
#include	<rzcmd\rzr009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_wei_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤計量特性構造体データをテキストに変換する。			*/
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
rz_print_wei_spec(
	struct fmt_wei_spec	*txt,
	struct rzr009t		*pkt
	)
{
	itoh((unsigned short) pkt->stable_time, (char *)&txt->stable_time );
	ctoh( pkt->stable_count, (char *)&txt->stable_count );
	itoh((unsigned short) pkt->zero_limit_cnt, (char *)&txt->zero_limit_cnt );
	ctoh( pkt->zero_interval, (char *)&txt->zero_interval );
	txt->stable_cmp_cnt = hex_ascii( pkt->stable_cmp_cnt );
	itoh((unsigned short) pkt->empty_weight, (char *)&txt->empty_weight );
	txt->range = hex_ascii( pkt->range );
	ctoh( pkt->smpl_interval, (char *)&txt->smpl_interval );
	ctoh( pkt->iir_reset_cnt, (char *)&txt->iir_reset_cnt );
	txt->anticipation = hex_ascii( pkt->anticipation );
	return(++txt);
}
