/*======================================================================
 * File name    : rzi139.c
 * Original	: rzi039.c
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニタ情報　テキスト解読（拡張１）ステイル・ロックタイム仕様
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi139.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi139.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi139.c 1.2 1998/04/04 11:33:35 hiroki
 *Log: rzi139.c
 * リビジョン 1.2  1998/04/04  11:33:35  hiroki
 * ctoi -> htoc に修正。
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
#include	<rzcmd\rzi139.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ステイル・ロックタイム・テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_stale_time(
	struct fmt_stale_time	*buf
	)
{
	rzi139s.stale_time   = htoi((char *)buf->stale_time);
	rzi139s.stale_mode   = ascii_hex(buf->stale_mode);
	rzi139s.stale_tmr    = htoi((char *)buf->stale_tmr);
	rzi139s.stale_dcnt   = htoc((char *)buf->stale_dcnt);
	return(0);
}
