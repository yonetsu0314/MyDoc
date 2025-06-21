/*======================================================================
 * File name    : rzr005.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr005.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzr005.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/09/25  14:57
 * 符号拡張が正しく行われるようにする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr005.cv  2.1  93/11/02 19:15:02  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr005.cv
 * Revision 2.1  93/11/02 19:15:02  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr005.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_infd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	供給機の制御仕様を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信テキスト・バッファへのポインタ	    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int		
rz_scan_infd_spec( 
	struct fmt_infd_spec	*buf
	)
{
	unsigned char *p;
	register int	i;

	rzr005s.nof_head = htoc((char *) buf->nof_head );
	rzr005s.nof_sig = ascii_hex( buf->nof_sig );
	/*ここから可変長データ処理*/
	p = (BYTE_PTR)&buf->sig[0];
	for( i = 0; i < rzr005s.nof_sig; i++ ){
		rzr005s.sig[i].sig_assign_ptn = htol((char *) p );
			p += sizeof( buf->sig[i].sig_assign_ptn );
		rzr005s.sig[i].infd_detector = ascii_hex( *p );
			p += sizeof( buf->sig[i].infd_detector );
		rzr005s.sig[i].transient_int = htoc((char *) p );
			p += sizeof( buf->sig[i].transient_int );
		rzr005s.sig[i].feed_intrpt = htoc((char *) p );
			p += sizeof( buf->sig[i].feed_intrpt );
	}
	rzr005s.nof_cf = ascii_hex( *p );
		p += sizeof( buf->nof_cf );
	for( i = 0; i < rzr005s.nof_cf; i++ ){
		rzr005s.cf_assign_ptn[i] = htol((char *) p );
			p += sizeof( buf->cf_assign_ptn[i] );
	}
	rzr005s.pconst = (short)htoi((char *) p );
		p += sizeof( buf->pconst );
	rzr005s.iconst = (short)htoi((char *) p );
		p += sizeof( buf->iconst );
	rzr005s.dconst = (short)htoi((char *) p );
		p += sizeof( buf->dconst );
	rzr005s.filter_sw = (short)htoi((char *) p );
		p += sizeof( buf->filter_sw );
	rzr005s.filter_const = (short)htoi((char *) p );
		p += sizeof( buf->filter_const );
	return( 0 );
}
