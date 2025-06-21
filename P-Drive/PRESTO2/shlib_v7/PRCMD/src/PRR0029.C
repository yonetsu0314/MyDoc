/*======================================================================
 * File name	: prr0029.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0029.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0029.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/rcs/dzr209.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzr209.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0029.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_hop_spec2						*/
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
pr_scan_hop_spec2(
	struct fmt_hop_spec2	*buf
	)
{
	BYTE	*p;
	int	i;

	p = (BYTE_PTR)buf->nof_head;
	prr0029s.nof_head = htoc( (char *)p );
	p += sizeof( buf->nof_head );
	prr0029s.hop_assign_ptn = htol( (char *)p );
	p += sizeof( buf->hop_assign_ptn );
	prr0029s.open_time = htoi( (char *)p );
	p += sizeof( buf->open_time );
	prr0029s.hopper_id = ascii_hex( *p );
	p += sizeof( buf->hopper_id );
	prr0029s.hopper_no = ascii_hex( *p );
	p += sizeof( buf->hopper_no );
	prr0029s.stop_mode = ascii_hex( *p );
	p += sizeof( buf->stop_mode );
	prr0029s.actuator_type = ascii_hex( *p );
	p += sizeof( buf->actuator_type );
	if(prr0029s.actuator_type == ACT_SM) {
		prr0029s.cpx.sm.phase_type = ascii_hex( *p );
		p += sizeof( buf->cpx.sm.phase_type );
		prr0029s.cpx.sm.full_pow_tm = htoi( (char *)p );
		p += sizeof( buf->cpx.sm.full_pow_tm );
		prr0029s.cpx.sm.ignore_pulse = htoi( (char *)p );
		p += sizeof( buf->cpx.sm.ignore_pulse );
		prr0029s.cpx.sm.cam_slit = htoc( (char *)p );
		p += sizeof( buf->cpx.sm.cam_slit );
		prr0029s.cpx.sm.hold_mode = ascii_hex( *p );
		p += sizeof( buf->cpx.sm.hold_mode );
		prr0029s.cpx.sm.brake_time = htoc( (char *)p );
		p += sizeof( buf->cpx.sm.brake_time );
		prr0029s.cpx.sm.error_pulse = htoc( (char *)p );
		p += sizeof( buf->cpx.sm.error_pulse );
		prr0029s.cpx.sm.num_section = htoc( (char *)p );
		p += sizeof( buf->cpx.sm.num_section );
		prr0029s.cpx.sm.mode_flags = htoc( (char *)p );
		p += sizeof( buf->cpx.sm.mode_flags );
		prr0029s.cpx.sm.start_pps = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.start_pps );
		prr0029s.cpx.sm.fullstep = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.fullstep );
		prr0029s.cpx.sm.pitch = (short)htoi( (char *)p );
		p += sizeof( buf->cpx.sm.pitch );
		for(i = 0 ; i < prr0029s.cpx.sm.num_section ; i++) {
			prr0029s.cpx.sm.msd[i].amp_n = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].amp_n );
			prr0029s.cpx.sm.msd[i].sec_flag_power = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].sec_flag_power );
			prr0029s.cpx.sm.msd[i].time_hold = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].time_hold );
			prr0029s.cpx.sm.msd[i].num_ss_start = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].num_ss_start );
			prr0029s.cpx.sm.msd[i].num_ss_stop = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].num_ss_stop );
			prr0029s.cpx.sm.msd[i].max_pps = (short)htoi( (char *)p );
			p += sizeof( buf->cpx.sm.msd[i].max_pps );
		}
	} else {
		prr0029s.cpx.cb.osc = htoi( (char *)p );
		p += sizeof( buf->cpx.cb.osc );
		prr0029s.cpx.cb.cl_drive_tm = htoi( (char *)p );
		p += sizeof( buf->cpx.cb.cl_drive_tm );
		prr0029s.cpx.cb.cl_on_delay = htoc( (char *)p );
		p += sizeof( buf->cpx.cb.cl_on_delay );
		prr0029s.cpx.cb.br_on_delay = htoc( (char *)p );
		p += sizeof( buf->cpx.cb.br_on_delay );
		prr0029s.cpx.cb.br_time = htoc( (char *)p );
		p += sizeof( buf->cpx.cb.br_time );
	}
	return(0);
}
