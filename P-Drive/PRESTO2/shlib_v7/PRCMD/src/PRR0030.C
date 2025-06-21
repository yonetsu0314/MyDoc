/*======================================================================
 * File name	: prr0030.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0030.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0030.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/rcs/dzr210.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzr210.c
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
#include	<prcmd\prr0029.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_hop_spec2						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dzhop_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_hop_spec2(
	struct fmt_hop_spec2	*txt,
	struct prr0029t		*pkt
	)
{
	BYTE		*buf;
	int		i;

	buf = (BYTE_PTR)txt->nof_head;
	ctoh( pkt->nof_head, (char *)buf );
	buf += sizeof( txt->nof_head );
	ltoh ( pkt->hop_assign_ptn, (char *)buf );
	buf += sizeof( txt->hop_assign_ptn );
	itoh( pkt->open_time, (char *)buf );
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
		itoh( pkt->cpx.sm.full_pow_tm, (char *)buf );
		buf += sizeof( txt->cpx.sm.full_pow_tm );
		itoh( pkt->cpx.sm.ignore_pulse, (char *)buf );
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
		ctoh( pkt->cpx.sm.mode_flags, (char *)buf );
		buf += sizeof( txt->cpx.sm.mode_flags );
		itoh( (unsigned short)pkt->cpx.sm.start_pps, (char *)buf );
		buf += sizeof( txt->cpx.sm.start_pps );
		itoh( (unsigned short)pkt->cpx.sm.fullstep, (char *)buf );
		buf += sizeof( txt->cpx.sm.fullstep );
		itoh( (unsigned short)pkt->cpx.sm.pitch, (char *)buf );
		buf += sizeof( txt->cpx.sm.pitch );
		for( i = 0 ; i < pkt->cpx.sm.num_section ; i++ ) {
			itoh( (unsigned short)pkt->cpx.sm.msd[i].amp_n, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].amp_n );
			itoh( (unsigned short)pkt->cpx.sm.msd[i].sec_flag_power, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].sec_flag_power );
			itoh( (unsigned short)pkt->cpx.sm.msd[i].time_hold, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].time_hold );
			itoh( (unsigned short)pkt->cpx.sm.msd[i].num_ss_start, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].num_ss_start );
			itoh( (unsigned short)pkt->cpx.sm.msd[i].num_ss_stop, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].num_ss_stop );
			itoh( (unsigned short)pkt->cpx.sm.msd[i].max_pps, (char *)buf );
			buf += sizeof( txt->cpx.sm.msd[i].max_pps );
		}
	} else {
		itoh( pkt->cpx.cb.osc, (char *)buf );
		buf += sizeof( txt->cpx.cb.osc );
		itoh( pkt->cpx.cb.cl_drive_tm, (char *)buf );
		buf += sizeof( txt->cpx.cb.cl_drive_tm );
		ctoh( pkt->cpx.cb.cl_on_delay, (char *)buf );
		buf += sizeof( txt->cpx.cb.cl_on_delay );
		ctoh( pkt->cpx.cb.br_on_delay, (char *)buf );
		buf += sizeof( txt->cpx.cb.br_on_delay );
		ctoh( pkt->cpx.cb.br_time, (char *)buf );
		buf += sizeof( txt->cpx.cb.br_time );
	}
	return(buf);
}
