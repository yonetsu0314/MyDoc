/*======================================================================
 * File name    : rzi017.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi017.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi017.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi017.cv  2.2  94/04/12 13:43:14  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi017.cv
 * Revision 2.2  94/04/12 13:43:14  kabumoto
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
#define		STORAGE
#include	<rzcmd\rzi017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_error							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	エラーテキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_error(
	struct fmt_error	*buf
	)
{
	rzi017s.ch_id = ascii_hex(buf->ch_id);
	rzi017s.nof_head = htoc((char *)buf->nof_head);
	rzi017s.error = ascii_hex(buf->error);
	rzi017s.ph_ptn = htol((char *)buf->ph_ptn);
	rzi017s.wh_ptn = htol((char *)buf->wh_ptn);
	rzi017s.bh_ptn = htol((char *)buf->bh_ptn);
	rzi017s.rs_ptn = htoc((char *)buf->rs_ptn);
	rzi017s.dth_ptn = htoc((char *)buf->dth_ptn);
	rzi017s.th_ptn = htoc((char *)buf->th_ptn);
	rzi017s.df_ptn = htoc((char *)buf->df_ptn);
	return(0);
}
