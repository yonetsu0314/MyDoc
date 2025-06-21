/*======================================================================
 * File name    : rzr011.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr011.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr011.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr011.cv  2.1  93/11/02 19:16:10  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr011.cv
 * Revision 2.1  93/11/02 19:16:10  ryoji
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
#include	<rzcmd\rzr011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cbcal_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算特性テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cbcal_spec(
	struct fmt_cbcal_spec	*buf
	)
{
	rzr011s.nof_head       	= htoc((char *) buf->nof_head );
	rzr011s.wh_active 	= htol((char *) buf->wh_active );
	rzr011s.bh_active 	= htol((char *) buf->bh_active );
	rzr011s.compensation	= htoc((char *) buf->compensation );
	rzr011s.renew_compensation = ascii_hex( buf->renew_compensation );
	rzr011s.calc_mode 	= ascii_hex( buf->calc_mode );
	rzr011s.mix_mode 	= ascii_hex( buf->mix_mode );
	rzr011s.over_scale 	= ascii_hex( buf->over_scale );
	rzr011s.over_weight 	= ascii_hex( buf->over_weight );
	rzr011s.hop_error 	= ascii_hex( buf->hop_error );
	rzr011s.az_error 	= ascii_hex( buf->az_error );
	rzr011s.err_rej_type 	= ascii_hex( buf->err_rej_type );
	return(0);
}
