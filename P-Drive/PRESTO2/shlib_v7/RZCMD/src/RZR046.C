/*======================================================================
 * File name    : rzr046.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	手動計算テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr046.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr046.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr046.cv  2.1  93/11/02 19:23:32  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr046.cv
 * Revision 2.1  93/11/02 19:23:32  ryoji
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
#include	<rzcmd\rzr045.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_test_calc						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	テスト組み合わせ計算構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_test_calc)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_test_calc(
	struct fmt_test_calc	*txt,
	struct rzr045t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	return(++txt);
}
