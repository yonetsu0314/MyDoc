/*======================================================================
 * File name    : rzr093.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *　マルチ連動パラメータ２テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr093.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr093.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr093.c 1.2 1997/12/08 20:15:07 itami
 *Log: rzr093.c
 * リビジョン 1.2  1997/12/08  20:15:07  itami
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
#include	<rzcmd\rzr093.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/* 	rz_scan_mlt_parm2						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動パラメータ２のテキストを構造体にセーブする。    　　	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_mlt_parm2 (
	struct fmt_mlt_parm2	*buf
	)
{
	rzr093s.ch_id		= ascii_hex( buf->ch_id);
	rzr093s.multi_onoff	= htoc( (char *)buf->multi_onoff);
	rzr093s.multi_enter    	= htoc( (char *)buf->multi_enter);
	rzr093s.combi_delay    	= htoc( (char *)buf->combi_delay);
	rzr093s.consec_mode    	= htoc( (char *)buf->consec_mode);
	rzr093s.mlt_dump_mode	= htoc( (char *)buf->mlt_dump_mode);
	rzr093s.interface    	= htoc( (char *)buf->interface);
	rzr093s.speed    	= htoc( (char *)buf->speed);
	rzr093s.conv_delay    	= htoc( (char *)buf->conv_delay);
	rzr093s.conv_on_time    = htoc( (char *)buf->conv_on_time);
	rzr093s.ds_delay    	= htoc( (char *)buf->ds_delay);
	rzr093s.ds_on_time    	= htoc( (char *)buf->ds_on_time);
	rzr093s.ds_sht_cnt    	= htoc( (char *)buf->ds_sht_cnt);
	rzr093s.mlt_dump_conf	= htoc( (char *)buf->mlt_dump_conf);
	rzr093s.p_ds_delay    	= htoc( (char *)buf->p_ds_delay);
	rzr093s.p_ds_on_time    = htoc( (char *)buf->p_ds_on_time);
	rzr093s.p_ds_sht_cnt    = htoc( (char *)buf->p_ds_sht_cnt);

	return(0);
}
