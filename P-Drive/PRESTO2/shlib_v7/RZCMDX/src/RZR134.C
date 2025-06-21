/*======================================================================
 * File name    : rzr134.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	初期零点調整テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr134.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr134.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr134.c 1.3 1998/08/07 20:20:35 hiroe
 *Log: rzr134.c
 * リビジョン 1.3  1998/08/07  20:20:35  hiroe
 * coff -> coef
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr133.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_init_zero_flag						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	初期零点調整構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_zero_adj)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_init_zero_flag(
	struct fmt_init_zero_flag	*txt,
	struct rzr133t		*pkt
	)
{
	txt->init_zero_flag = hex_ascii( pkt->init_zero_flag );
	itoh( pkt->ad_coef, (char *)&txt->ad_coef );
	itoh( pkt->da_coef, (char *)&txt->da_coef );
	itoh( pkt->range, (char *)&txt->range );
	return(++txt);
}
