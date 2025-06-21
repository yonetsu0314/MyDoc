/*======================================================================
 * File name    : rzr013.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr013.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr013.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr013.cv  2.1  93/11/02 19:15:48  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr013.cv
 * Revision 2.1  93/11/02 19:15:48  ryoji
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
#include	<rzcmd\rzr013.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_hop_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ホッパ駆動特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_hop_spec(
	struct fmt_hop_spec	*buf
	)
{
	P3		*p;
	int		i;

	rzr013s.nof_head       	= htoc((char *) buf->nof_head );
	rzr013s.hop_assign_ptn 	= htol((char *) buf->hop_assign_ptn );
	rzr013s.open_time      	= (unsigned short)htoi((char *) buf->open_time );
	rzr013s.hopper_id      	= ascii_hex( buf->hopper_id );
	rzr013s.hopper_no      	= ascii_hex( buf->hopper_no );
	rzr013s.actuator_type  	= ascii_hex( buf->actuator_type );
	rzr013s.phase_type     	= ascii_hex( buf->phase_type );
	rzr013s.stop_mode 	= ascii_hex( buf->stop_mode );
	rzr013s.speed       	= (unsigned short)htoi((char *) buf->speed );
	rzr013s.ignore_pulse  	= (unsigned short)htoi((char *) buf->ignore_pulse );
	rzr013s.cam_slit  	= htoc((char *) buf->cam_slit );
	rzr013s.br_cl_delay    	= htoc((char *) buf->br_cl_delay );
	rzr013s.cl_br_delay     = htoc((char *) buf->cl_br_delay );
	rzr013s.brake_time     	= htoc((char *) buf->brake_time );
	rzr013s.error_pulse     = htoc((char *) buf->error_pulse );
	rzr013s.nof_pattern	= htoc((char *) buf->nof_pattern );
	p = &buf->ary[0];
	for(i = 0 ; i < rzr013s.nof_pattern ; i++, p++) {
		rzr013s.ary[i].pulse_mltp  = htoc((char *) &p->pulse_mltp );
		rzr013s.ary[i].repeat  	   = htoc((char *) &p->repeat );
	}
	return(0);
}
