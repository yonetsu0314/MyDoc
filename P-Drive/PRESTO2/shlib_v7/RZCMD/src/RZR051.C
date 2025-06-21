/*======================================================================
 * File name    : rzr051.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr051.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr051.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr051.cv  2.1  93/11/02 19:24:08  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr051.cv
 * Revision 2.1  93/11/02 19:24:08  ryoji
 * 第５版対応
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
#include	<rzcmd\rzr051.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	テスト駆動開始テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_tstdrv_run(
	struct fmt_tstdrv_run	*buf
	)
{
	rzr051s.ch_id = ascii_hex(buf->ch_id);
	rzr051s.nof_head = htoc((char *)&buf->nof_head);
	rzr051s.df_ptn = htoc((char *)&buf->df_ptn);
	rzr051s.rf_ptn = htol((char *)&buf->rf_ptn);
	rzr051s.ph_ptn = htol((char *)&buf->ph_ptn);
	rzr051s.wh_ptn = htol((char *)&buf->wh_ptn);
	rzr051s.wh2_ptn = htol((char *)&buf->wh2_ptn);
	rzr051s.bh_ptn = htol((char *)&buf->bh_ptn);
	rzr051s.rs_ptn = htoc((char *)&buf->rs_ptn);
	rzr051s.dth_ptn = htoc((char *)&buf->dth_ptn);
	rzr051s.th_ptn = htoc((char *)&buf->th_ptn);
	rzr051s.drv_cyc = ascii_hex( buf->drv_cyc );
	return(0);
}
