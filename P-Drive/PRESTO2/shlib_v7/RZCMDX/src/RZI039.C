/*======================================================================
 * File name    : rzi039.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニタ情報　テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi039.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi039.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi039.c 1.6 1996/07/12 20:09:36 hiroki
 *Log: rzi039.c
 * リビジョン 1.6  1996/07/12  20:09:36  hiroki
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
#include	<rzcmd\rzi039.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＴＡモニター・テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_ata_monitor(
	struct fmt_ata_monitor	*buf
	)
{
	rzi039s.ch_id        = ascii_hex( buf->ch_id);
	rzi039s.ata_enable   = ascii_hex(buf->ata_enable);
	rzi039s.ata_timer    = htoi((char *)buf->ata_timer);
	rzi039s.ata_mode     = ascii_hex(buf->ata_mode);
	rzi039s.ata_tgt      = htol((char *)buf->ata_tgt);
	rzi039s.ata_def_time = htoi((char *)buf->ata_def_time);
	rzi039s.ata_cwb      = htoi((char *)buf->ata_cwb);

	return(0);
}
