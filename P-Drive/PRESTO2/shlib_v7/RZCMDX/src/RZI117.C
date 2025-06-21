/*======================================================================
 * File name    : rzi117.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生サブテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi117.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi117.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi117.c 1.2 1994/09/17 14:03:50 hiroe
 *Log: rzi117.c
 * リビジョン 1.2  1994/09/17  14:03:50  hiroe
 * rzi017 -> rzi117
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
#include	<rzcmd\rzi117.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_error_sub						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	エラーサブテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_error_sub(
	struct fmt_error_sub	*buf
	)
{
	rzi117s.ch_id = ascii_hex(buf->ch_id);
	rzi117s.nof_head = htoc((char *)buf->nof_head);
	rzi117s.error = htoc((char *)buf->error);
	rzi117s.error_ptn = htol((char *)buf->error_ptn);
	return(0);
}
