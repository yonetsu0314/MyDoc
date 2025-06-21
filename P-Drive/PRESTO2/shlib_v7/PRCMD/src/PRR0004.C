/*======================================================================
 * File name	: prr0004.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量機仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0004.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr004.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr004.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0003.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_mch_construct						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	計量機の構成仕様データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信テキスト・バッファへのポインタ			*/
/*	*pkt	: パラメータ・パケットへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_mch_construct)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
pr_print_mch_construct(
	struct fmt_mch_construct	*txt,
	struct prr0003t			*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->head_construct = hex_ascii( pkt->head_construct );
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	txt->model_id = hex_ascii( pkt->model_id );
	txt->nof_df = hex_ascii( pkt->nof_df );
	buf = (BYTE_PTR)txt->df_assign_ptn[0];

	for( i = 0; i < (int)pkt->nof_df; i++ ) {
		ltoh(pkt->df_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->df_assign_ptn[i] );
	}

	ctoh( pkt->nof_af, (char *)buf );
	buf += sizeof( txt->nof_af );
	for( i = 0; i < (int)pkt->nof_af; i++ ) {
		ltoh(pkt->af_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->af_assign_ptn[i] );
	}

	ctoh( pkt->nof_rs, (char *)buf );
	buf += sizeof( txt->nof_rs );
	for( i = 0; i < (int)pkt->nof_rs; i++ ) {
		ltoh(pkt->rs[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->rs[i].assign_ptn );
		*buf = hex_ascii( pkt->rs[i].drv_type );
		buf += sizeof( txt->rs[i].drv_type );
		*buf = hex_ascii( pkt->rs[i].direction );
		buf += sizeof( txt->rs[i].direction );
		*buf = hex_ascii( pkt->rs[i].level );
		buf += sizeof( txt->rs[i].level );
		ctoh( pkt->rs[i].port, (char *)buf );
		buf += sizeof( txt->rs[i].port );
	}
	ctoh( pkt->nof_dth, (char *)buf );
	buf += sizeof( txt->nof_dth );
	for( i = 0; i < (int)pkt->nof_dth; i++ ) {
		ltoh(pkt->dth[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->dth[i].assign_ptn );
		*buf = hex_ascii( pkt->dth[i].drv_type );
		buf += sizeof( txt->dth[i].drv_type );
		*buf = hex_ascii( pkt->dth[i].direction );
		buf += sizeof( txt->dth[i].direction );
		*buf = hex_ascii( pkt->dth[i].level );
		buf += sizeof( txt->dth[i].level );
		ctoh( pkt->dth[i].port, (char *)buf );
		buf += sizeof( txt->dth[i].port );
	}
	ctoh( pkt->nof_th, (char *)buf );
	buf += sizeof( txt->nof_th );
	for( i = 0; i < (int)pkt->nof_th; i++ ) {
		ltoh(pkt->th[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->th[i].assign_ptn );
		*buf = hex_ascii( pkt->th[i].drv_type );
		buf += sizeof( txt->th[i].drv_type );
		*buf = hex_ascii( pkt->th[i].direction );
		buf += sizeof( txt->th[i].direction );
		*buf = hex_ascii( pkt->th[i].level );
		buf += sizeof( txt->th[i].level );
		ctoh( pkt->th[i].port, (char *)buf );
		buf += sizeof( txt->th[i].port );
	}
	*buf = hex_ascii( pkt->nof_duc_net );
	buf += sizeof( txt->nof_duc_net );
	for( i = 0; i < (int)pkt->nof_duc_net; i++ ){
		ltoh(pkt->duc_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->duc_assign_ptn[i] );
	}
	return(buf);
}
