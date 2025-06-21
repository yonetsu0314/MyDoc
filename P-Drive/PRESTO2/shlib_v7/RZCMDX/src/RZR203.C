/*======================================================================
 * File name	: rzr203.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ構成仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr203.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr203.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr203.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr203.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr203.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_af_construct						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ構成仕様を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
rz_scan_af_construct( 
	struct fmt_af_construct	*buf
	)
{
	unsigned char *p;
	register int	i;

	rzr203s.nof_af = htoc( (char *)buf->nof_af );
	p = (BYTE_PTR)buf->af_assign_ptn[0];
	for( i = 0; i < rzr203s.nof_af; i++ ){
		rzr203s.af_assign_ptn[i] = htol((char *)p );
		p += sizeof( buf->af_assign_ptn[i] );
	}
	return( 0 );
}
