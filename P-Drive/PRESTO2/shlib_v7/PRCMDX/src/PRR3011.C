/*======================================================================
 * File name	: prr3011.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算拡張パラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr3011.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr3011.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzr311.c 1.1 1996/07/15 12:10:25 hiroki Exp
 *log: rzr311.c
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
#include	<prcmd\prr3011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_max_scale						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	max_scale							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_max_scale(
	struct fmt_max_scale	*buf
	)
{
	prr3011s.max_scale = ascii_hex(buf->max_scale);
	return(0);
}
