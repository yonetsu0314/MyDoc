/*======================================================================
 * File name    : rzr085.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数要求テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr085.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr085.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr085.c 1.1 1997/01/08 09:00:13 chimura
 *Log: rzr085.c
 * リビジョン 1.1  1997/01/08  09:00:13  chimura
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
#define		STORAGE
#include	<rzcmd\rzr085.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_span_call						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	スパン係数要求テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_span_call(
	struct fmt_span_call	*buf
	)
{
	rzr085s.pwb_id = ascii_hex(buf->pwb_id);
	return(0);
}
