/*======================================================================
 * File name    : nzr015.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr015.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr015.c 1.1 1999/05/27 16:52:05 kawamura
 *Log: nzr015.c
 * リビジョン 1.1  1999/05/27  16:52:05  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzr015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_fd_spec							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ駆動特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_fd_spec(
	struct fmt_fd_spec_nz	*buf
	)
{
	nzr015s.nof_head = htoc( (char *)buf->nof_head );
	nzr015s.fd_assign_ptn = htol( (char *)buf->fd_assign_ptn );
	nzr015s.feeder_id   = ascii_hex( buf->feeder_id );
	nzr015s.min_amp	    = ascii_hex( buf->min_amp );
	nzr015s.max_amp	    = ascii_hex( buf->max_amp );
	nzr015s.multiplier  = ascii_hex( buf->multiplier );
	nzr015s.phase_type  = ascii_hex( buf->phase_type );
	nzr015s.opt_sw_ctrl = ascii_hex( buf->opt_sw_ctrl );
	nzr015s.shutter_ctrl= ascii_hex( buf->shutter_ctrl );
	nzr015s.shutter_off_delay = htoc( (char *)buf->shutter_off_delay );
	nzr015s.slow_start  = ascii_hex( buf->slow_start );
	return(0);
}
