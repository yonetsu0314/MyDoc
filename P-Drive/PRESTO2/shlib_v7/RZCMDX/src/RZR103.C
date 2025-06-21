/*======================================================================
 * File name    : rzr113.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *　計量機の構成仕様拡張テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr103.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr103.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr103.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzr103.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
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
#include	<rzcmd\rzr103.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/* 	rz_scan_bucket_inf						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量機の構成仕様拡張のテキストを構造体にセーブする。   　　	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_bucket_inf (
	struct fmt_bucket_inf	*buf
	)
{
	rzr103s.nof_wh		= htoc( (char *)buf->nof_wh	);
	rzr103s.nof_bb		= htoc( (char *)buf->nof_bb	);
	rzr103s.nof_mb		= htoc( (char *)buf->nof_mb	);
	rzr103s.nof_wherr	= htoc( (char *)buf->nof_wherr	);
	rzr103s.nof_whwh	= htoc( (char *)buf->nof_whwh	);
	rzr103s.nof_whbb	= htoc( (char *)buf->nof_whbb	);
	rzr103s.nof_bbwh	= htoc( (char *)buf->nof_bbwh	);

	return(0);
}
