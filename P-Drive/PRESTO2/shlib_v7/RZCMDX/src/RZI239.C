/*======================================================================
 * File name    : rzi239.c
 * Original	: rzi139.c
 *----------------------------------------------------------------------
 * Function	:
 * ＡＴＡ98モニタ情報　テキスト解読（拡張２）ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi239.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi239.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi239.c 1.1 1999/04/06 10:58:21 hiroki
 *Log: rzi239.c
 * リビジョン 1.1  1999/04/06  10:58:21  hiroki
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
#include	<rzcmd\rzi239.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_stale98_time						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ステイル・ロックタイム・テキストを構造体にセーブする。		*/
/*	（ＡＴＡ９８仕様）						*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_stale98_time(
	struct fmt_stale98_time	*buf
	)
{
	rzi239s.stale98_time   = htoi((char *)buf->stale98_time);
	rzi239s.stale98_mode   = ascii_hex(buf->stale98_mode);
	rzi239s.stale98_tmr    = htoi((char *)buf->stale98_tmr);
	rzi239s.stale98_dcnt   = htoc((char *)buf->stale98_dcnt);
	rzi239s.stale98_SDEnable   = ascii_hex( buf->stale98_SDEnable);
	rzi239s.stale98_drain_mode = ascii_hex(buf->stale98_drain_mode);
	rzi239s.stale98_drain_tmr  = htoi((char *)buf->stale98_drain_tmr);

	return(0);
}
