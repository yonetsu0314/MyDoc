/*======================================================================
 * File name	: nzr053.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr053.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr053.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr053.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr053.c
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
#include	<nzcmd\nzr053.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_drv_cycle						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ駆動周期設定を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
nz_scan_drv_cycle( 
	struct fmt_drv_cycle	*buf
	)
{
	unsigned char	*p;
	register int	i;

	nzr053s.nof_rf = htoc( (char *)buf->nof_rf );
	p = (BYTE_PTR)buf->rf_drv_cycle[0];
	for( i = 0; i < nzr053s.nof_rf; i++ ){
		nzr053s.rf_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->rf_drv_cycle[i] );
	}
	nzr053s.nof_df = htoc( (char *)p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < nzr053s.nof_df; i++ ){
		nzr053s.df_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->df_drv_cycle[i] );
	}
	nzr053s.nof_cf = htoc( (char *)p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < nzr053s.nof_cf; i++ ){
		nzr053s.cf_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->cf_drv_cycle[i] );
	}
	return( 0 );
}
