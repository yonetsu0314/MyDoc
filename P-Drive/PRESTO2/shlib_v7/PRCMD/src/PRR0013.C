/*======================================================================
 * File name	: prr0013.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0013.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0013.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr013.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr013.c
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
#include	<prcmd\prr0013.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_hop_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_hop_spec(
	struct fmt_hop_spec	*buf
	)
{
	PR00130	*p;
	int	i;

	prr0013s.nof_head = htoc( (char *)buf->nof_head );
	prr0013s.hop_assign_ptn = htol( (char *)buf->hop_assign_ptn );
	prr0013s.open_time = htoi( (char *)buf->open_time );
	prr0013s.hopper_id = ascii_hex( buf->hopper_id );
	prr0013s.hopper_no = ascii_hex( buf->hopper_no );
	prr0013s.actuator_type = ascii_hex( buf->actuator_type );
	prr0013s.phase_type = ascii_hex( buf->phase_type );
	prr0013s.stop_mode = ascii_hex( buf->stop_mode );
	prr0013s.speed = htoi( (char *)buf->speed );
	prr0013s.ignore_pulse = htoi( (char *)buf->ignore_pulse );
	prr0013s.cam_slit = htoc( (char *)buf->cam_slit );
	prr0013s.br_cl_delay = htoc( (char *)buf->br_cl_delay );
	prr0013s.cl_br_delay = htoc( (char *)buf->cl_br_delay );
	prr0013s.brake_time = htoc( (char *)buf->brake_time );
	prr0013s.error_pulse = htoc( (char *)buf->error_pulse );
	prr0013s.nof_pattern = htoc( (char *)buf->nof_pattern );
	p = buf->ary;
	for(i = 0 ; i < prr0013s.nof_pattern ; i++, p++) {
		prr0013s.ary[i].pulse_mltp = htoc( (char *)p->pulse_mltp );
		prr0013s.ary[i].repeat = htoc( (char *)p->repeat );
	}
	return(0);
}
