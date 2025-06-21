/*======================================================================
 * File name    : rzr009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr009.cv  2.1  93/11/02 19:16:34  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr009.cv
 * Revision 2.1  93/11/02 19:16:34  ryoji
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
#include	<rzcmd\rzr009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_wei_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤計量特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_wei_spec(
	struct fmt_wei_spec	*buf
	)
{
	rzr009s.stable_time    	= (unsigned short)htoi((char *)buf->stable_time );
	rzr009s.stable_count   	= htoc((char *) buf->stable_count );
	rzr009s.zero_limit_cnt	= (unsigned short)htoi((char *)buf->zero_limit_cnt );
	rzr009s.zero_interval  	= htoc((char *) buf->zero_interval );
	rzr009s.stable_cmp_cnt 	= ascii_hex( buf->stable_cmp_cnt );
	rzr009s.empty_weight	= (unsigned short)htoi((char *)buf->empty_weight );
	rzr009s.range 		= ascii_hex( buf->range );
	rzr009s.smpl_interval 	= htoc((char *)buf->smpl_interval );
	rzr009s.iir_reset_cnt  	= htoc((char *)buf->iir_reset_cnt );
	rzr009s.anticipation	= ascii_hex( buf->anticipation );
	return(0);
}
