/*======================================================================
 * File name	: prr0005.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0005.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr005.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr005.c
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
#include	<prcmd\prr0005.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_infd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	供給機の制御仕様を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信テキスト・バッファへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_infd_spec(
	struct fmt_infd_spec	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0005s.nof_head = htoc((char *) buf->nof_head );
	prr0005s.nof_sig = ascii_hex( buf->nof_sig );
	p = (BYTE_PTR)buf->sig;
	for( i = 0; i < (int)prr0005s.nof_sig; i++ ) {
		prr0005s.sig[i].sig_assign_ptn = htol( (char *)p );
		p += sizeof( buf->sig[i].sig_assign_ptn );
		prr0005s.sig[i].infd_detector = ascii_hex( *p );
		p += sizeof( buf->sig[i].infd_detector );
		prr0005s.sig[i].transient_int = htoc( (char *)p );
		p += sizeof( buf->sig[i].transient_int );
		prr0005s.sig[i].feed_intrpt = htoc( (char *)p );
		p += sizeof( buf->sig[i].feed_intrpt );
		prr0005s.sig[i].targ = htoi( (char *)p );
		p += sizeof(buf->sig[i].targ );
		prr0005s.sig[i].upper = htoc( (char *)p );
		p += sizeof(buf->sig[i].upper );
		prr0005s.sig[i].lower = htoc( (char *)p );
		p += sizeof(buf->sig[i].lower );
	}
	prr0005s.nof_cf = ascii_hex( *p );
	p += sizeof( buf->nof_cf );
	for( i = 0; i < (int)prr0005s.nof_cf; i++ ) {
		prr0005s.cf_assign_ptn[i] = htol( (char *)p );
		p += sizeof( buf->cf_assign_ptn[i] );
	}
	prr0005s.pconst = htoi( (char *)p );
	p += sizeof( buf->pconst );
	prr0005s.iconst = htoi( (char *)p );
	p += sizeof( buf->iconst );
	prr0005s.dconst = htoi( (char *)p );
	p += sizeof( buf->dconst );
	prr0005s.filter_sw = htoi( (char *)p );
	p += sizeof( buf->filter_sw );
	prr0005s.filter_const = htoi( (char *)p );
	p += sizeof( buf->filter_const );
	return( 0 );
}
