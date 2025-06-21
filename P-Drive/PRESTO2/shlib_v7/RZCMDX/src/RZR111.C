/*======================================================================
 * File name    : rzr111.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算拡張パラメータ（１）テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr111.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr111.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr111.c 1.1 1994/06/22 17:34:40 hiroe
 *Log: rzr111.c
 * リビジョン 1.1  1994/06/22  17:34:40  hiroe
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
#include	<rzcmd\rzr111.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_approval						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	検定に必要なテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_approval(
	struct fmt_approval	*buf
	)
{
	rzr111s.approval_flag 	= ascii_hex( buf->approval_flag );
	rzr111s.vol_flag 	= ascii_hex( buf->vol_flag );
	rzr111s.max_weight     	= htoi( (char *)buf->max_weight );
	rzr111s.min_weight 	= htoi( (char *)buf->min_weight );
	return(0);
}
