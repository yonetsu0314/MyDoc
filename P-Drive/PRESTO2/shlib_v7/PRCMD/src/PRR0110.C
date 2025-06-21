/*======================================================================
 * File name	: prr0110.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動周期設定テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0110.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0110.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr054.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr054.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0109.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_drv_cycle						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動周期設定データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信テキスト・バッファへのポインタ			*/
/*	*pkt	: パラメータ・パケットへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_drv_cycle)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
pr_print_drv_cycle(
	struct fmt_drv_cycle	*txt,
	struct prr0109t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	buf = (BYTE_PTR)txt->nof_rf;
	ctoh( pkt->nof_rf, (char *)buf );
	buf += sizeof(txt->nof_rf);
	for( i = 0; i < pkt->nof_rf; i++ ) {
		itoh( (unsigned short)pkt->rf_drv_cycle[i], (char *)buf );
		buf += sizeof( txt->rf_drv_cycle[i] );
	}
	ctoh( pkt->nof_df, (char *)buf );
	buf += sizeof( txt->nof_df );
	for( i = 0; i < pkt->nof_df; i++ ) {
		itoh( (unsigned short)pkt->df_drv_cycle[i], (char *)buf );
		buf += sizeof( txt->df_drv_cycle[i] );
	}
	ctoh( pkt->nof_af, (char *)buf );
	buf += sizeof( txt->nof_af );
	for( i = 0; i < pkt->nof_af; i++ ) {
		itoh( (unsigned short)pkt->af_drv_cycle[i], (char *)buf );
		buf += sizeof( txt->af_drv_cycle[i] );
	}
	ctoh( pkt->nof_cf, (char *)buf );
	buf += sizeof( txt->nof_cf );
	for( i = 0; i < pkt->nof_cf; i++ ) {
		itoh( (unsigned short)pkt->cf_drv_cycle[i], (char *)buf );
		buf += sizeof( txt->cf_drv_cycle[i] );
	}
	return(buf);

}
