/*======================================================================
 * File name    : rzr068.c
 * Original	: rzr066.c Rev.1.1
 *----------------------------------------------------------------------
 * Function	:
 *	金属検出機テストのテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr068.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr068.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr068.c 1.1 1994/07/08 13:30:50 hiroe
 *Log: rzr068.c
 * リビジョン 1.1  1994/07/08  13:30:50  hiroe
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
#include	<rzcmd\rzr067.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_metal_text						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	金属検出機テストの構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_metal_text)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_metal_text(
	struct fmt_metal_text	*txt,
	struct rzr067t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	txt->function = hex_ascii(pkt->function);
	return(++txt);
}
