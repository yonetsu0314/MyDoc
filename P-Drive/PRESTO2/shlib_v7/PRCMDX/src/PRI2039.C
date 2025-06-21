/*======================================================================
 * File name	: pzi2039.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 * ＡＴＡ98モニタ情報　テキスト解読  ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri2039.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri2039.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi239.c 1.1 1999/04/06 10:58:21 hiroki Exp
 *log: rzi239.c
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
#include	<prcmd\pri2039.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_stale98_time						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ステイル・ロックタイム・テキストを構造体にセーブする。		*/
/*	（ＡＴＡ９８仕様）						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_stale98_time(
	struct fmt_stale98_time	*buf
	)
{
	pri2039s.stale98_time       = htoi((char *)buf->stale98_time);
	pri2039s.stale98_mode       = ascii_hex(buf->stale98_mode);
	pri2039s.stale98_tmr        = htoi((char *)buf->stale98_tmr);
	pri2039s.stale98_dcnt       = htoc((char *)buf->stale98_dcnt);
	pri2039s.stale98_SDEnable   = ascii_hex( buf->stale98_SDEnable);
	pri2039s.stale98_drain_mode = ascii_hex(buf->stale98_drain_mode);
	pri2039s.stale98_drain_tmr  = htoi((char *)buf->stale98_drain_tmr);
	return(0);
}
