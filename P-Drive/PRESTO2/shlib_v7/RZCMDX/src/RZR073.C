/*======================================================================
 * File name    : rzr073.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ダミー計量開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr073.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr073.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr073.c 1.1 1995/04/24 20:31:08 sakagami
 *Log: rzr073.c
 * リビジョン 1.1  1995/04/24  20:31:08  sakagami
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr073.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_dummy_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ダミー計量開始テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_dummy_weight(
	struct fmt_dummy_weight	*buf
	)
{
	rzr073s.ch_id = ascii_hex(buf->ch_id);
	return(0);
}
