/*======================================================================
 * File name	: prr0109.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0109.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0109.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr053.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr053.c
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
#include	<prcmd\prr0109.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_drv_cycle						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動周期設定を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_drv_cycle(
	struct fmt_drv_cycle	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0109s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_drv_cycle[0];
	for( i = 0; i < prr0109s.nof_rf; i++ ) {
		prr0109s.rf_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_drv_cycle[i] );
	}
	prr0109s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < prr0109s.nof_df; i++ ) {
		prr0109s.df_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_drv_cycle[i] );
	}
	prr0109s.nof_af = htoc( (char *)p );
	p += sizeof( buf->nof_af );
	for( i = 0; i < prr0109s.nof_af; i++ ) {
		prr0109s.af_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->af_drv_cycle[i] );
	}
	prr0109s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < prr0109s.nof_cf; i++ ) {
		prr0109s.cf_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_drv_cycle[i] );
	}
	return( 0 );
}
