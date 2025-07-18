/*======================================================================
 * File name    : rzr096.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr096.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr096.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr096.c 1.2 1997/12/04 13:34:36 itami
 *Log: rzr096.c
 * リビジョン 1.2  1997/12/04  13:34:36  itami
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
#include	<rzcmd\rzr095.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_mlt_ext						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動拡張パラメータをテキストに変換する。	    		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *rz_print_mlt_ext(
	struct fmt_mlt_ext	*txt,
	struct rzr095t		*pkt
	)
{
	txt->s_cmd = hex_ascii( pkt->s_cmd );
	ctoh( pkt->p1, (char *)&txt->p1 );
	ctoh( pkt->p2, (char *)&txt->p2 );
	ctoh( pkt->p3, (char *)&txt->p3 );
	ctoh( pkt->p4, (char *)&txt->p4 );
	return(++txt);
}
