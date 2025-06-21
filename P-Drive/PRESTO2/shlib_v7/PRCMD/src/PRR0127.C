/*======================================================================
 * File name	: prr0127.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数設定データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0127.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0127.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0127.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_hop_drive_set						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動回数設定データテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_hop_drive_set(
	struct fmt_hop_drive_set	*buf
	)
{
	prr0127s.hopper_id = ascii_hex(buf->hopper_id);
	prr0127s.hopper_no = htoc((char *)buf->hopper_no);
	prr0127s.nof_times = htol((char *)buf->nof_times );
	return 0;
}
