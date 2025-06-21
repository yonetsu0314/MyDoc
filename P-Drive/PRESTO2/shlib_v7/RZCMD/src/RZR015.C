/*======================================================================
 * File name    : rzr015.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr015.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr015.cv  2.1  93/11/02 19:17:20  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr015.cv
 * Revision 2.1  93/11/02 19:17:20  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_fd_spec							*/
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
rz_scan_fd_spec(
	struct fmt_fd_spec	*buf
	)
{
	rzr015s.nof_head = htoc((char *) buf->nof_head );
	rzr015s.fd_assign_ptn = htol((char *) buf->fd_assign_ptn );
	rzr015s.feeder_id   = ascii_hex( buf->feeder_id );
	rzr015s.min_amp	    = ascii_hex( buf->min_amp );
	rzr015s.max_amp	    = ascii_hex( buf->max_amp );
	rzr015s.multiplier  = ascii_hex( buf->multiplier );
	rzr015s.phase_type  = ascii_hex( buf->phase_type );
	rzr015s.opt_sw_ctrl = ascii_hex( buf->opt_sw_ctrl );
	rzr015s.shutter_ctrl= ascii_hex( buf->shutter_ctrl );
	rzr015s.shutter_off_delay = htoc((char *) buf->shutter_off_delay );

	return(0);
}
