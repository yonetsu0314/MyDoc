/*======================================================================
 * File name    : rzi118.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生サブテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi118.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi118.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi118.c 1.2 1994/09/17 14:18:19 hiroe
 *Log: rzi118.c
 * リビジョン 1.2  1994/09/17  14:18:19  hiroe
 * errorの型をunsigned charにする。
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
#include	<rzcmd\rzi117.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_error_sub						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	エラー構造体のサブデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_error)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
rz_print_error_sub(
	struct fmt_error_sub	*txt,
	struct rzi117t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	ctoh((unsigned char)pkt->error, (char *)txt->error);
	ltoh(pkt->error_ptn, (char *)txt->error_ptn);
	return(++txt);
}
