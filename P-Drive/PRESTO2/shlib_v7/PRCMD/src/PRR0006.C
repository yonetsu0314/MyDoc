/*======================================================================
 * File name	: prr0006.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0006.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr006.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr006.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0005.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_infd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	供給機の制御仕様データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信テキスト・バッファへのポインタ			*/
/*	*pkt	: パラメータ・パケットへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_infd_spec)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
pr_print_infd_spec(
	struct fmt_infd_spec	*txt,
	struct prr0005t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	ctoh( pkt->nof_head, (char *)txt->nof_head );
	txt->nof_sig = hex_ascii( pkt->nof_sig );
	buf = (BYTE_PTR)(txt->sig[0].sig_assign_ptn);

	for( i = 0; i < (int)pkt->nof_sig; i++ ) {
		ltoh( pkt->sig[i].sig_assign_ptn, (char *)buf );
		buf += sizeof( txt->sig[i].sig_assign_ptn );
		*buf = hex_ascii( pkt->sig[i].infd_detector );
		buf += sizeof( txt->sig[i].infd_detector );
		ctoh( pkt->sig[i].transient_int, (char *)buf );
		buf += sizeof( txt->sig[i].transient_int );
		ctoh( pkt->sig[i].feed_intrpt, (char *)buf );
		buf += sizeof( txt->sig[i].feed_intrpt );
		itoh( pkt->sig[i].targ, (char *)buf );
		buf += sizeof( txt->sig[i].targ );
		ctoh( pkt->sig[i].upper, (char *)buf );
		buf += sizeof( txt->sig[i].upper );
		ctoh( pkt->sig[i].lower, (char *)buf );
		buf += sizeof( txt->sig[i].lower );
	}
	*buf = hex_ascii( pkt->nof_cf );
	buf += sizeof( txt->nof_cf );
	for( i = 0; i < (int)pkt->nof_cf; i++ ) {
		ltoh( pkt->cf_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->cf_assign_ptn[i] );
	}
	itoh( (unsigned short) pkt->pconst, (char *)buf );
	buf += sizeof( txt->pconst );
	itoh( (unsigned short) pkt->iconst, (char *)buf );
	buf += sizeof( txt->iconst );
	itoh( (unsigned short) pkt->dconst, (char *)buf );
	buf += sizeof( txt->dconst );
	itoh( (unsigned short) pkt->filter_sw, (char *)buf );
	buf += sizeof( txt->filter_sw );
	itoh( (unsigned short) pkt->filter_const, (char *)buf );
	buf += sizeof( txt->filter_const );
	return(buf);
}
