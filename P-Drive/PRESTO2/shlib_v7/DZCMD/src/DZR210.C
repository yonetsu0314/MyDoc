/*======================================================================
 * File name    : dzr210.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr210.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr210.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr210.cv  1.5  94/03/16 14:01:50  kabumoto
 *Log: P:/LIB/DZCMD/SRC/RCS/dzr210.cv
 * Revision 1.5  94/03/16 14:01:50  kabumoto
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
#include	<dzcmd\dzr209.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	dz_print_hop_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ホッパ駆動特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dzhop_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
dz_print_hop_spec(
	struct fmt_dzhop_spec	*txt,
	struct dzr209t		*pkt
	)
{
	DZR2093T	*p;
	BYTE		*buf, c;
	int		i;

	buf = (PTR)txt->nof_head;
	ctoh( pkt->nof_head, (char *)buf );
	buf += sizeof( txt->nof_head );
	ltoh ( (unsigned long)pkt->hop_assign_ptn, (char *)buf );
	buf += sizeof( txt->hop_assign_ptn );
	itoh( (unsigned short)pkt->open_time, (char *)buf );
	buf += sizeof( txt->open_time );
	*buf = hex_ascii( pkt->hopper_id );
	buf += sizeof( txt->hopper_id );
	*buf = hex_ascii( pkt->hopper_no );
	buf += sizeof( txt->hopper_no );
	*buf = hex_ascii( pkt->stop_mode );
	buf += sizeof( txt->stop_mode );
	*buf = hex_ascii( pkt->actuator_type );
	buf += sizeof( txt->actuator_type );
	if(pkt->actuator_type == ACT_SM) {
	    *buf = hex_ascii( pkt->cpx.sm.phase_type );
	    buf += sizeof( txt->cpx.sm.phase_type );
	    itoh( (unsigned short)pkt->cpx.sm.full_pow_tm, (char *)buf );
	    buf += sizeof( txt->cpx.sm.full_pow_tm );
	    itoh( (unsigned short)pkt->cpx.sm.ignore_pulse, (char *)buf );
	    buf += sizeof( txt->cpx.sm.ignore_pulse );
	    ctoh( pkt->cpx.sm.cam_slit, (char *)buf );
	    buf += sizeof( txt->cpx.sm.cam_slit );
	    *buf = hex_ascii( pkt->cpx.sm.hold_mode );
	    buf += sizeof( txt->cpx.sm.hold_mode );
	    ctoh( pkt->cpx.sm.brake_time, (char *)buf );
	    buf += sizeof( txt->cpx.sm.brake_time );
	    ctoh( pkt->cpx.sm.error_pulse, (char *)buf );
	    buf += sizeof( txt->cpx.sm.error_pulse );
	    ctoh( pkt->cpx.sm.num_section, (char *)buf );
	    buf += sizeof( txt->cpx.sm.num_section );
	    memcpy( (void *)&c, (void *)&pkt->cpx.sm.mode_flags, sizeof(pkt->cpx.sm.mode_flags) );
	    ctoh( c, (char *)buf);
	    buf += sizeof( txt->cpx.sm.mode_flags );
	    itoh( (unsigned short)pkt->cpx.sm.start_pps, (char *)buf );
	    buf += sizeof( txt->cpx.sm.start_pps );
	    itoh( (unsigned short)pkt->cpx.sm.fullstep, (char *)buf );
	    buf += sizeof( txt->cpx.sm.fullstep );
	    itoh( (unsigned short)pkt->cpx.sm.pitch, (char *)buf );
	    buf += sizeof( txt->cpx.sm.pitch );
	    for(i= 0, p= &pkt->cpx.sm.msd[0]; i< pkt->cpx.sm.num_section ; i++, p++) {
		itoh( (unsigned short)p->amp_n, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].amp_n );
		itoh( (unsigned short)p->sec_flag_power, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].sec_flag_power );
	 	itoh( (unsigned short)p->time_hold, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].time_hold );
		itoh( (unsigned short)p->num_ss_start, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].num_ss_start );
		itoh( (unsigned short)p->num_ss_stop, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].num_ss_stop );
		itoh( (unsigned short)p->max_pps, (char *)buf );
		buf += sizeof( txt->cpx.sm.msd[i].max_pps );
	    }
	}
	else {
	    itoh( (unsigned short)pkt->cpx.cb.osc, (char *)buf );
	    buf += sizeof( txt->cpx.cb.osc );
	    itoh( (unsigned short)pkt->cpx.cb.cl_drive_tm, (char *)buf );
	    buf += sizeof( txt->cpx.cb.cl_drive_tm );
 	    ctoh( pkt->cpx.cb.cl_on_delay, (char *)buf );
	    buf += sizeof( txt->cpx.cb.cl_on_delay );
	    ctoh( pkt->cpx.cb.br_on_delay, (char *)buf );
	    buf += sizeof( txt->cpx.cb.br_on_delay );
	    ctoh( pkt->cpx.cb.br_time, (char *)buf );
	    buf += sizeof( txt->cpx.cb.br_time );
	}
	return((PTR)buf);
}
