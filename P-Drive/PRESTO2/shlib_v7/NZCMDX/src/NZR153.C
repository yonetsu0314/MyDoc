/*======================================================================
 * File name	: nzr153.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ駆動周期設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmdx/src/RCS/nzr153.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr153.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmdx/src/rcs/nzr153.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr153.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzr153.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_af_drv_cycle						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ駆動周期設定を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
nz_scan_af_drv_cycle( 
	struct fmt_af_drv_cycle	*buf
	)
{
	unsigned char	*p;
	register int	i;

	nzr153s.nof_af = htoc( (char *)buf->nof_af );
	p = (BYTE_PTR)buf->af_drv_cycle[0];
	for( i = 0; i < nzr153s.nof_af; i++ ){
		nzr153s.af_drv_cycle[i] = (short)htoi( (char *)p );
		p += sizeof( buf->af_drv_cycle[i] );
	}
	return( 0 );
}
