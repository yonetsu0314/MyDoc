/*======================================================================
 * File name	: prr0031.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断データ要求テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0031.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0031.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzr205.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzr205.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0031.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_diag_req						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤診断データ要求テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_diag_req(
	struct fmt_diag_req	*buf
	)
{
	prr0031s.ch_id = ascii_hex(buf->ch_id);
	return(0);
}
