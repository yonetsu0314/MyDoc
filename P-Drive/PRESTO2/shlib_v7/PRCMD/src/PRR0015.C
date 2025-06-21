/*======================================================================
 * File name	: prr0015.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0015.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr015.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr015.c
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
#include	<prcmd\prr0015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_fd_spec							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_fd_spec(
	struct fmt_fd_spec	*buf
	)
{
	prr0015s.nof_head = htoc( (char *)buf->nof_head );
	prr0015s.fd_assign_ptn = htol( (char *)buf->fd_assign_ptn );
	prr0015s.feeder_id = ascii_hex( buf->feeder_id );
	prr0015s.min_amp = ascii_hex( buf->min_amp );
	prr0015s.max_amp = ascii_hex( buf->max_amp );
	prr0015s.multiplier = ascii_hex( buf->multiplier );
	prr0015s.phase_type = ascii_hex( buf->phase_type );
	prr0015s.opt_sw_ctrl = ascii_hex( buf->opt_sw_ctrl );
	prr0015s.shutter_ctrl = ascii_hex( buf->shutter_ctrl );
	prr0015s.shutter_off_delay = htoc( (char *)buf->shutter_off_delay );
	prr0015s.slow_start = ascii_hex( buf->slow_start );
	prr0015s.stop_ptn = ascii_hex( buf->stop_ptn );
	return(0);
}
