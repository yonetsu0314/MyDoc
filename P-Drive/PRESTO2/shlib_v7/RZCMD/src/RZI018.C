/*======================================================================
 * File name    : rzi018.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi018.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi018.cv  2.2  94/04/12 13:43:50  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi018.cv
 * Revision 2.2  94/04/12 13:43:50  kabumoto
 * AFV異常コードをDFエラービットパターンに変更する
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
#include	<rzcmd\rzi017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_error							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	エラー構造体のデータをテキストに変換する。			*/
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
rz_print_error(
	struct fmt_error	*txt,
	struct rzi017t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	txt->error = hex_ascii(pkt->error);
	ltoh((unsigned long)pkt->ph_ptn, (char *)txt->ph_ptn);
	ltoh((unsigned long)pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh((unsigned long)pkt->bh_ptn, (char *)txt->bh_ptn);
	ctoh(pkt->rs_ptn, (char *)txt->rs_ptn);
	ctoh(pkt->dth_ptn, (char *)txt->dth_ptn);
	ctoh(pkt->th_ptn, (char *)txt->th_ptn);
	ctoh(pkt->df_ptn, (char *)txt->df_ptn);
	return(++txt);
}
