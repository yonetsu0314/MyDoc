/*======================================================================
 * File name    : rzi017a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用異常発生テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi017a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi017a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi017a.c 1.1 1995/11/21 09:23:24 hiroe
 *Log: rzi017a.c
 * リビジョン 1.1  1995/11/21  09:23:24  hiroe
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
#define		STORAGE
#include	<rzcmd\rzi017a.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_error_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用エラーテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_error_v3(
	struct fmt_error_v3	*buf
	)
{
	rzi017s.ch_id = ascii_hex(buf->ch_id);
	rzi017s.nof_head = htoc((char *)buf->nof_head);
	rzi017s.error = htoc((char *)buf->error);
	rzi017s.ph_ptn = htol((char *)buf->ph_ptn);
	rzi017s.wh_ptn = htol((char *)buf->wh_ptn);
	rzi017s.bh_ptn = htol((char *)buf->bh_ptn);
	rzi017s.rs_ptn = htoc((char *)buf->rs_ptn);
	rzi017s.dth_ptn = htoc((char *)buf->dth_ptn);
	rzi017s.th_ptn = htoc((char *)buf->th_ptn);
	rzi017s.df_ptn = htoc((char *)buf->df_ptn);
	rzi017s.duc_inf = ascii_hex(buf->duc_inf);
	rzi017s.exc_inf = ascii_hex(buf->exc_inf);
	return 0;
}
