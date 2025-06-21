/*======================================================================
 * File name	: pri0052.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0052.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0052.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi052.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi052.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0051.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_resonance						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ共振周期応答データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信テキスト・バッファへのポインタ			*/
/*	*pkt	: パラメータ・パケットへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_resonance)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
pr_print_resonance(
	struct fmt_resonance	*txt,
	struct pri0051t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	buf = (BYTE_PTR)txt->nof_rf;
	ctoh( pkt->nof_rf, (char *)buf );
	buf += sizeof(txt->nof_rf);
	for( i = 0; i < pkt->nof_rf; i++ ) {
		itoh( (unsigned short)pkt->rf_res_cycle[i], (char *)buf );
		buf += sizeof( txt->rf_res_cycle[i] );
	}
	ctoh( pkt->nof_df, (char *)buf );
	buf += sizeof( txt->nof_df );
	for( i = 0; i < pkt->nof_df; i++ ) {
		itoh( (unsigned short)pkt->df_res_cycle[i], (char *)buf );
		buf += sizeof( txt->df_res_cycle[i] );
	}
	ctoh( pkt->nof_af, (char *)buf );
	buf += sizeof( txt->nof_af );
	for( i = 0; i < pkt->nof_af; i++ ) {
		itoh( (unsigned short)pkt->af_res_cycle[i], (char *)buf );
		buf += sizeof( txt->af_res_cycle[i] );
	}
	ctoh( pkt->nof_cf, (char *)buf );
	buf += sizeof( txt->nof_cf );
	for( i = 0; i < pkt->nof_cf; i++ ) {
		itoh( (unsigned short)pkt->cf_res_cycle[i], (char *)buf );
		buf += sizeof( txt->cf_res_cycle[i] );
	}
	return(buf);

}
