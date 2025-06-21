/*======================================================================
 * File name	: prr0121.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	リレー駆動要求データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0121.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0121.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
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
#include	<prcmd\prr0121.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_ryout_check						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	リレー駆動要求データテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_ryout_check(
	struct fmt_ryout_check	*buf
	)
{
	prr0121s.brd_no = ascii_hex(buf->brd_no);
	prr0121s.drive_ptn = htoc((char *)buf->drive_ptn);
	prr0121s.drive_delay = htoc((char *)buf->drive_delay);
	prr0121s.drive_time = htoc((char *)buf->drive_time);
	return(0);
}
