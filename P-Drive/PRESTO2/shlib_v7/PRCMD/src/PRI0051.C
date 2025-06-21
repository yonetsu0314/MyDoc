/*======================================================================
 * File name	: pri0051.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0051.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0051.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi051.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi051.c
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
#include	<prcmd\pri0051.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_resonance						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ共振周期応答を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_resonance(
	struct fmt_resonance	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0051s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_res_cycle[0];
	for( i = 0; i < pri0051s.nof_rf; i++ ) {
		pri0051s.rf_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_res_cycle[i] );
	}
	pri0051s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < pri0051s.nof_df; i++ ) {
		pri0051s.df_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_res_cycle[i] );
	}
	pri0051s.nof_af = htoc( (char *)p );
	p += sizeof( buf->nof_af );
	for( i = 0; i < pri0051s.nof_af; i++ ) {
		pri0051s.af_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->af_res_cycle[i] );
	}
	pri0051s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < pri0051s.nof_cf; i++ ) {
		pri0051s.cf_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_res_cycle[i] );
	}
	return( 0 );
}
