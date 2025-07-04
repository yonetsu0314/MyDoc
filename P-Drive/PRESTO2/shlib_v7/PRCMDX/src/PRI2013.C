/*======================================================================
 * File name	: pri2013.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	本体状態情報拡張パラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/pri2013.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri2013.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri2013.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_fsv_condition						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_fsv_condition(
	struct fmt_fsv_condition	*buf
	)
{
	int	i;

	pri2013s.nof_ch = ascii_hex(buf->nof_ch);
	/*ここから可変長データ処理*/
	for(i = 0; i < pri2013s.nof_ch; i++) {
		pri2013s.cnd[i] = ascii_hex( buf->cnd[i] );
	}
	return(0);
}
