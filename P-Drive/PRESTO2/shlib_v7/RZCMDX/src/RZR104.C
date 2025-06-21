/*======================================================================
 * File name    : rzr104.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量機の構成仕様拡張テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr104.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr104.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr104.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzr104.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
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
#include	<rzcmd\rzr103.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_bucket_inf						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量機の構成仕様拡張をテキストに変換する。	    		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *rz_print_bucket_inf(
	struct fmt_bucket_inf	*txt,
	struct rzr103t		*pkt
	)
{
	ctoh( pkt->nof_wh, 	(char *)&txt->nof_wh );
	ctoh( pkt->nof_bb, 	(char *)&txt->nof_bb );
	ctoh( pkt->nof_mb, 	(char *)&txt->nof_mb );
	ctoh( pkt->nof_wherr, 	(char *)&txt->nof_wherr );
	ctoh( pkt->nof_whwh, 	(char *)&txt->nof_whwh );
	ctoh( pkt->nof_whbb, 	(char *)&txt->nof_whbb );
	ctoh( pkt->nof_bbwh, 	(char *)&txt->nof_bbwh );
	return(++txt);
}
