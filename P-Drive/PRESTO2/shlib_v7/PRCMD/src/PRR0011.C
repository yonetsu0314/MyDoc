/*======================================================================
 * File name	: prr0011.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0011.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0011.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr011.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr011.c
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
#include	<prcmd\prr0011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_cbcal_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	組み合わせ計算特性テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_cbcal_spec(
	struct fmt_cbcal_spec	*buf
	)
{
	prr0011s.nof_head = htoc( (char *)buf->nof_head );
	prr0011s.wh_active = htol( (char *)buf->wh_active );
	prr0011s.bh_active = htol( (char *)buf->bh_active );
	prr0011s.compensation = htoc( (char *)buf->compensation );
	prr0011s.renew_compensation = ascii_hex( buf->renew_compensation );
	prr0011s.calc_mode = ascii_hex( buf->calc_mode );
	prr0011s.mix_mode = ascii_hex( buf->mix_mode );
	prr0011s.over_scale = ascii_hex( buf->over_scale );
	prr0011s.over_weight = ascii_hex( buf->over_weight );
	prr0011s.hop_error = ascii_hex( buf->hop_error );
	prr0011s.az_error = ascii_hex( buf->az_error );
	prr0011s.stable = ascii_hex( buf->stable );
	prr0011s.re_check = ascii_hex( buf->re_check );
	prr0011s.th_error = ascii_hex( buf->th_error );
	prr0011s.nof_stop_head = htoc( (char *)buf->nof_stop_head );
	prr0011s.nof_stop_cycle = htoc( (char *)buf->nof_stop_cycle );
	prr0011s.err_rej_type = ascii_hex( buf->err_rej_type );
	prr0011s.multi_cal = ascii_hex( buf->multi_cal );
	prr0011s.approval_flag = ascii_hex( buf->approval_flag );
	prr0011s.max_weight = htoi( (char *)buf->max_weight );
	prr0011s.min_weight = htoi( (char *)buf->min_weight );
	return(0);
}
