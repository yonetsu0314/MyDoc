/*======================================================================
 * File name	: prr0014.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0014.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0014.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr014.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr014.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0013.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_hop_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動特性構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_hop_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_hop_spec(
	struct fmt_hop_spec	*txt,
	struct prr0013t		*pkt
	)
{
	BYTE		*buf;
	PRR00130T	*p;
	int		i;

	ctoh( pkt->nof_head, (char *)txt->nof_head );
	ltoh( pkt->hop_assign_ptn, (char *)txt->hop_assign_ptn );
	itoh( pkt->open_time, (char *)txt->open_time );
	txt->hopper_id = hex_ascii( pkt->hopper_id );
	txt->hopper_no = hex_ascii( pkt->hopper_no );
	txt->actuator_type = hex_ascii( pkt->actuator_type );
	txt->phase_type = hex_ascii( pkt->phase_type );
	txt->stop_mode = hex_ascii( pkt->stop_mode );
	itoh( pkt->speed, (char *)txt->speed );
	itoh( pkt->ignore_pulse, (char *)txt->ignore_pulse );
	ctoh( pkt->cam_slit, (char *)txt->cam_slit );
	ctoh( pkt->br_cl_delay, (char *)txt->br_cl_delay );
	ctoh( pkt->cl_br_delay, (char *)txt->cl_br_delay );
	ctoh( pkt->brake_time, (char *)txt->brake_time );
	ctoh( pkt->error_pulse, (char *)txt->error_pulse );
	ctoh( pkt->nof_pattern, (char *)txt->nof_pattern );
	buf = (BYTE_PTR)txt->ary;
	for(i = 0, p = (PTR)pkt->ary; i < pkt->nof_pattern ; i++, p++) {
		ctoh( p->pulse_mltp, (char *)buf);
		buf += sizeof(txt->ary[i].pulse_mltp);
		ctoh( p->repeat, (char *)buf);
		buf += sizeof(txt->ary[i].repeat);
	}
	return(buf);
}
