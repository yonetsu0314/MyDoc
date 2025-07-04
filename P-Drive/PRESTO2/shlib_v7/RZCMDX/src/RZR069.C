/*======================================================================
 * File name    : rzr069.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	サンプル排出仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr069.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr069.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzr069.c 1.1 1994/09/23 20:17:52 hiroe
 *Log: rzr069.c
 * リビジョン 1.1  1994/09/23  20:17:52  hiroe
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
#include	<rzcmd\rzr069.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_sample_dump						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	サンプル排出仕様を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int		
rz_scan_sample_dump( 
	struct fmt_sample_dump	*buf
	)
{
	rzr069s.ch_id = ascii_hex( buf->ch_id );
	rzr069s.sect_assign_ptn = htol( (char *)buf->sect_assign_ptn );
	return( 0 );
}
