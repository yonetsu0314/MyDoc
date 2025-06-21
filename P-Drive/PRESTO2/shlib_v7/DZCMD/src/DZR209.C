/*======================================================================
 * File name    : dzr209.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr209.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: dzr209.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2004/10/28  16:50
 * エアー駆動時、キャストミスでホッパが開かない不具合を修正。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/09/25  14:40
 * 符号拡張時、正しく符号拡張が行われるようにする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr209.cv  1.6  94/03/16 14:01:52  kabumoto
 *Log: P:/LIB/DZCMD/SRC/RCS/dzr209.cv
 * Revision 1.6  94/03/16 14:01:52  kabumoto
 * co_velo_nをsec_flag_powerに変更
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<dzcmd\dzr209.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	dz_scan_hop_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ホッパ駆動特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
dz_scan_hop_spec(
	struct fmt_dzhop_spec	*buf
	)
{
	BYTE		*p, c;
	int		i;

	p = (PTR)buf->nof_head;
	dzr209s.nof_head = htoc( (char *)p );
	p += sizeof( buf->nof_head );
	dzr209s.hop_assign_ptn = htol( (char *)p );
	p += sizeof( buf->hop_assign_ptn );
	dzr209s.open_time = (unsigned short)htoi( (char *)p );
	p += sizeof( buf->open_time );
	dzr209s.hopper_id = ascii_hex( *p );
	p += sizeof( buf->hopper_id );
	dzr209s.hopper_no = ascii_hex( *p );
	p += sizeof( buf->hopper_no );
	dzr209s.stop_mode = ascii_hex( *p );
	p += sizeof( buf->stop_mode );
	dzr209s.actuator_type = ascii_hex( *p );
	p += sizeof( buf->actuator_type );
	if(dzr209s.actuator_type == ACT_SM) {
	    dzr209s.cpx.sm.phase_type = (DZR2092E)ascii_hex( *p );
	    p += sizeof( buf->cpx.sm.phase_type );
	    dzr209s.cpx.sm.full_pow_tm = (unsigned short)htoi( (char *)p );
	    p += sizeof( buf->cpx.sm.full_pow_tm );
	    dzr209s.cpx.sm.ignore_pulse = (unsigned short)htoi( (char *)p );
	    p += sizeof( buf->cpx.sm.ignore_pulse );
	    dzr209s.cpx.sm.cam_slit = htoc( (char *)p );
	    p += sizeof( buf->cpx.sm.cam_slit );
	    dzr209s.cpx.sm.hold_mode = ascii_hex( *p );
	    p += sizeof( buf->cpx.sm.hold_mode );
	    dzr209s.cpx.sm.brake_time = htoc( (char *)p );
	    p += sizeof( buf->cpx.sm.brake_time );
	    dzr209s.cpx.sm.error_pulse = htoc( (char *)p );
	    p += sizeof( buf->cpx.sm.error_pulse );
	    dzr209s.cpx.sm.num_section = htoc( (char *)p );
	    p += sizeof( buf->cpx.sm.num_section );
	    c = htoc( (char *)p );
	    memcpy( (void *)&dzr209s.cpx.sm.mode_flags, (void *)&c, (size_t)sizeof(c) );
	    p += sizeof( buf->cpx.sm.mode_flags );
	    dzr209s.cpx.sm.start_pps = (short)htoi( (char *)p );
	    p += sizeof( buf->cpx.sm.start_pps );
	    dzr209s.cpx.sm.fullstep = (short)htoi( (char *)p );
	    p += sizeof( buf->cpx.sm.fullstep );
	    dzr209s.cpx.sm.pitch = (short)htoi( (char *)p );
	    p += sizeof( buf->cpx.sm.pitch );
	    for(i = 0 ; i < dzr209s.cpx.sm.num_section ; i++) {
		dzr209s.cpx.sm.msd[i].amp_n = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].amp_n );
		dzr209s.cpx.sm.msd[i].sec_flag_power = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].sec_flag_power );
		dzr209s.cpx.sm.msd[i].time_hold = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].time_hold );
		dzr209s.cpx.sm.msd[i].num_ss_start = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].num_ss_start );
		dzr209s.cpx.sm.msd[i].num_ss_stop = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].num_ss_stop );
		dzr209s.cpx.sm.msd[i].max_pps = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.msd[i].max_pps );
	    }
	}
	else {
	    dzr209s.cpx.cb.osc = htoi( (char *)p );
	    p += sizeof( buf->cpx.cb.osc );
	    dzr209s.cpx.cb.cl_drive_tm = htoi( (char *)p );
	    p += sizeof( buf->cpx.cb.cl_drive_tm );
	    dzr209s.cpx.cb.cl_on_delay = htoc( (char *)p );
	    p += sizeof( buf->cpx.cb.cl_on_delay );
	    dzr209s.cpx.cb.br_on_delay = htoc( (char *)p );
	    p += sizeof( buf->cpx.cb.br_on_delay );
	    dzr209s.cpx.cb.br_time = htoc( (char *)p );
	    p += sizeof( buf->cpx.cb.br_time );
	}
	return(0);
}
