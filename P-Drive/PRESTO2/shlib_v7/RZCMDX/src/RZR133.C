/*======================================================================
 * File name    : rzr133.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	初期零点調整テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr133.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr133.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr133.c 1.3 1998/08/07 20:20:18 hiroe
 *Log: rzr133.c
 * リビジョン 1.3  1998/08/07  20:20:18  hiroe
 * coff -> coef
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
#include	<rzcmd\rzr133.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_init_zero_flag(						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	初期零点調整テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_init_zero_flag(
	struct fmt_init_zero_flag	*buf
	)
{
	rzr133s.init_zero_flag = ascii_hex(buf->init_zero_flag);
	rzr133s.ad_coef = htoi((char *)buf->ad_coef);
	rzr133s.da_coef = htoi((char *)buf->da_coef);
	rzr133s.range = htoi((char *)buf->range);
	return(0);
}
