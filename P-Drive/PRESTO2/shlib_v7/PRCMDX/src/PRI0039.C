/*======================================================================
 * File name	: pri0039.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニタ情報　テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0039.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0039.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi039.c 1.6 1996/07/12 20:09:36 hiroki Exp
 *log: rzi039.c
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
#include	<prcmd\pri0039.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_ata_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＴＡモニター・テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_ata_monitor(
	struct fmt_ata_monitor	*buf
	)
{
	pri0039s.ch_id        = ascii_hex( buf->ch_id);
	pri0039s.ata_enable   = ascii_hex(buf->ata_enable);
	pri0039s.ata_timer    = htoi((char *)buf->ata_timer);
	pri0039s.ata_mode     = ascii_hex(buf->ata_mode);
	pri0039s.ata_tgt      = htol((char *)buf->ata_tgt);
	pri0039s.ata_def_time = htoi((char *)buf->ata_def_time);
	pri0039s.ata_cwb      = htoi((char *)buf->ata_cwb);
	return(0);
}
