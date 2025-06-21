/*======================================================================
 * File name    : dzr010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	AFV 拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr010.cv  1.3  94/05/19 09:00:24  sato
 *Log: p:/lib/dzcmd/src/rcs/dzr010.cv
 * Revision 1.3  94/05/19 09:00:24  sato
 * スパン分銅とセルタイプを追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#include	<dzcmd\dzr009.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	dz_print_wei_spec1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	AFV 拡張パラメータ構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	void *	: txt+sizeof(fmt_wei_spec1)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
dz_print_wei_spec1(
	struct fmt_wei_spec1	*txt,
	struct dzr009t		*pkt
	)
{
	txt->afv_mode	= hex_ascii(pkt->afv_mode);
	txt->filter	= hex_ascii(pkt->filter);
	txt->span_wt	= hex_ascii(pkt->span_wt);
	txt->cell_type	= hex_ascii(pkt->cell_type);

	return (char *)txt + sizeof(struct fmt_wei_spec1);
}
