/*======================================================================
 * File name    : rzi007.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補正値テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi007.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi007.cv  2.1  93/11/02 19:09:34  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi007.cv
 * Revision 2.1  93/11/02 19:09:34  ryoji
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
#include	<rzcmd\rzi007.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cmpn_value						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	単重自動更新結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cmpn_value(
	struct fmt_cmpn_value	*buf
	)
{
	rzi007s.cmpn_val = htoc((char *)buf->cmpn_val);
	return(0);
}
