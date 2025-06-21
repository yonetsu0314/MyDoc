/*======================================================================
 * File name	: nzi051.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi051.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi051.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi051.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzi051.c
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
#include	<nzcmd\nzi051.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_resonance						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ共振周期応答を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
nz_scan_resonance( 
	struct fmt_resonance	*buf
	)
{
	unsigned char	*p;
	register int	i;

	nzi051s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_res_cycle[0];
	for( i = 0; i < nzi051s.nof_rf; i++ ){
		nzi051s.rf_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_res_cycle[i] );
	}
	nzi051s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < nzi051s.nof_df; i++ ){
		nzi051s.df_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_res_cycle[i] );
	}
	nzi051s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < nzi051s.nof_cf; i++ ){
		nzi051s.cf_res_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_res_cycle[i] );
	}
	return( 0 );
}
