/*======================================================================
 * File name    : rzi038.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡモニター情報テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi038.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi038.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi038.c 1.3 1996/07/15 15:24:50 hiroki
 *Log: rzi038.c
 * リビジョン 1.3  1996/07/15  15:24:50  hiroki
 * ctoh -> itoh 
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
#include	<rzcmd\rzi037.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_dca_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＤＣＡモニター情報をテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_monitor)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_dca_monitor(
	struct fmt_dca_monitor	*txt,
	struct rzi037t		*pkt
	)
{
	itoh( (unsigned short)pkt->dca_sum_cnt, (char *)&txt->dca_sum_cnt );
	ltoh( pkt->bf_cbwt, (char *)&txt->bf_cbwt );
	ltoh( pkt->af_cbwt, (char *)&txt->af_cbwt );
	return(++txt);
}
