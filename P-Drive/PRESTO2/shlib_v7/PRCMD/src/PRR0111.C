/*======================================================================
 * File name	: prr0111.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0111.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0111.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr055.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr055.c
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
#include	<prcmd\prr0111.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_natural_freq						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ共振周期設定を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_natural_freq(
	struct fmt_natural_freq	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0111s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_natural_freq[0];
	for( i = 0; i < prr0111s.nof_rf; i++ ) {
		prr0111s.rf_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_natural_freq[i] );
	}
	prr0111s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < prr0111s.nof_df; i++ ) {
		prr0111s.df_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_natural_freq[i] );
	}
	prr0111s.nof_af = htoc( (char *)p );
	p += sizeof( buf->nof_af );
	for( i = 0; i < prr0111s.nof_af; i++ ) {
		prr0111s.af_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->af_natural_freq[i] );
	}
	prr0111s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < prr0111s.nof_cf; i++ ) {
		prr0111s.cf_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_natural_freq[i] );
	}
	return( 0 );
}
