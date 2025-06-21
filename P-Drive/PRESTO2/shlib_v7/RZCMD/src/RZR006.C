/*======================================================================
 * File name    : rzr006.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr006.cv  2.1  93/11/02 19:15:20  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr006.cv
 * Revision 2.1  93/11/02 19:15:20  ryoji
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
#include	<rzcmd\rzr005.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_infd_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	供給機の制御仕様データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_infd_spec)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
rz_print_infd_spec(
	struct fmt_infd_spec	*txt,
	struct rzr005t		*pkt
	)
{
	BYTE		*buf = (PTR)txt;
	int		i;

	ctoh( pkt->nof_head, (char *)txt->nof_head );
	txt->nof_sig = hex_ascii( pkt->nof_sig );
	buf = (BYTE_PTR)(txt->sig[0].sig_assign_ptn);
	/*ここから可変長データ処理*/
	for( i = 0; i < pkt->nof_sig; i++ ){
		ltoh((unsigned long) pkt->sig[i].sig_assign_ptn, (char *)buf ); 
		buf += sizeof( txt->sig[i].sig_assign_ptn );
		*buf = hex_ascii( pkt->sig[i].infd_detector );
		buf += sizeof( txt->sig[i].infd_detector );
	        ctoh( pkt->sig[i].transient_int, (char *)buf );
		buf += sizeof( txt->sig[i].transient_int );
		ctoh( pkt->sig[i].feed_intrpt, (char *)buf );
		buf += sizeof( txt->sig[i].feed_intrpt );
	}
	*buf = hex_ascii( pkt->nof_cf );
	buf += sizeof( txt->nof_cf );
	for( i = 0; i < pkt->nof_cf; i++ ){
		ltoh((unsigned long) pkt->cf_assign_ptn[i], (char *)buf );
			buf += sizeof( txt->cf_assign_ptn[i] );
	}
	itoh((unsigned short) pkt->pconst, (char *)buf );
	buf += sizeof( txt->pconst );
	itoh((unsigned short) pkt->iconst, (char *)buf );
	buf += sizeof( txt->iconst );
	itoh((unsigned short) pkt->dconst, (char *)buf );
	buf += sizeof( txt->dconst );
	itoh((unsigned short) pkt->filter_sw, (char *)buf );
	buf += sizeof( txt->filter_sw );
	itoh((unsigned short) pkt->filter_const, (char *)buf );
	buf += sizeof( txt->filter_const );

	return(buf);
}
