/*======================================================================
 * File name    : rzi044.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi044.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi044.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzi044.c 1.1 1997/01/08 09:00:06 chimura
 *Log: rzi044.c
 * リビジョン 1.1  1997/01/08  09:00:06  chimura
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
#include	<rzcmd\rzi043.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_span_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	スパン係数応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_span_adj)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_span_coeffi(
	struct fmt_span_coeffi	*txt,
	struct rzi043t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->pwb_id = hex_ascii( pkt->pwb_id );
	ltoh(pkt->date, (char *)&txt->date);
	ltoh(pkt->time, (char *)&txt->time);

	buf = (BYTE_PTR)&txt->span_coeffi[0];
	for(i= 0; i< sizeof(pkt->span_coeffi)/sizeof(pkt->span_coeffi[0]); i++) {
		itoh( pkt->span_coeffi[i], (char *)buf );
		buf += sizeof( txt->span_coeffi[0] );
	}

	return((PTR)buf);
}
