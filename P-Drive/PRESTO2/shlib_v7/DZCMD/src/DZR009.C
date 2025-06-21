/*======================================================================
 * File name    : dzr009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	AFV 拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr009.cv  1.2  94/05/19 08:58:46  sato
 *Log: p:/lib/dzcmd/src/rcs/dzr009.cv
 * Revision 1.2  94/05/19 08:58:46  sato
 * スパン分銅とセルタイプを追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<dzcmd\dzr009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	dz_scan_wei_spec1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	AFV 拡張パラメータテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
dz_scan_wei_spec1(
	struct fmt_wei_spec1	*buf
	)
{
	dzr009s.afv_mode    	= ascii_hex( buf->afv_mode );
	dzr009s.filter   	= ascii_hex( buf->filter );
	dzr009s.span_wt    	= ascii_hex( buf->span_wt );
	dzr009s.cell_type   	= ascii_hex( buf->cell_type );
	return(0);
}
