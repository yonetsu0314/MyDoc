/*======================================================================
 * File name    : dzr206.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断データ要求テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr206.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr206.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZR206.Cv  1.2  94/02/18 17:16:28  ryoji
 *Log: P:/LIB/DZCMD/SRC/RCS/DZR206.Cv
 * Revision 1.2  94/02/18 17:16:28  ryoji
 * 第１版対応
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
#include	<dzcmd\dzr205.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	dz_print_diag_req						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤診断データ要求の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_req_weidat)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
dz_print_diag_req(
	struct fmt_diag_req	*txt,
	struct dzr205t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->head_no, (char *)&txt->head_no);
	return(++txt);
}
