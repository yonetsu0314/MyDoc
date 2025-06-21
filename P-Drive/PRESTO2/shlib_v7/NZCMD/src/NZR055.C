/*======================================================================
 * File name	: nzr055.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr055.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr055.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr055.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr055.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzr055.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	nz_scan_natural_freq						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ共振周期設定を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
nz_scan_natural_freq( 
	struct fmt_natural_freq	*buf
	)
{
	unsigned char	*p;
	register int	i;

	nzr055s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_natural_freq[0];
	for( i = 0; i < nzr055s.nof_rf; i++ ){
		nzr055s.rf_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_natural_freq[i] );
	}
	nzr055s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < nzr055s.nof_df; i++ ){
		nzr055s.df_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_natural_freq[i] );
	}
	nzr055s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < nzr055s.nof_cf; i++ ){
		nzr055s.cf_natural_freq[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_natural_freq[i] );
	}
	return( 0 );
}
