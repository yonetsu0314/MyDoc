/*======================================================================
 * File name    : rzr004.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量機仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr004.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr004.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr004.cv  2.1  93/11/02 20:27:34  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr004.cv
 * Revision 2.1  93/11/02 20:27:34  ryoji
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
#include	<rzcmd\rzr003.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_mch_construct						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量機の構成仕様データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_mch_construct)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
rz_print_mch_construct(
	struct fmt_mch_construct	*txt,
	struct rzr003t			*pkt
	)
{
	BYTE		*buf = (PTR)txt;
	int		i;

	txt->head_construct = hex_ascii( pkt->head_construct );
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	txt->model_id = hex_ascii( pkt->model_id );
	txt->nof_df = hex_ascii( pkt->nof_df );
	buf = (BYTE_PTR)txt->df_assign_ptn[0];
	/*ここから可変長データ処理*/
	for( i = 0; i < pkt->nof_df; i++ ){
		ltoh((unsigned long) pkt->df_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->df_assign_ptn[i] );
	}
	*buf = hex_ascii( pkt->nof_rs );
	buf += sizeof( txt->nof_rs );
	for( i = 0; i < pkt->nof_rs; i++ ){
		ltoh((unsigned long) pkt->rs[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->rs[i].assign_ptn );
		*buf = hex_ascii( pkt->rs[i].drv_type );
		buf += sizeof( txt->rs[i].drv_type );
		*buf = hex_ascii( pkt->rs[i].direction );
		buf += sizeof( txt->rs[i].direction );
		*buf = hex_ascii( pkt->rs[i].level );
		buf += sizeof( txt->rs[i].level );
		*buf = hex_ascii( pkt->rs[i].port );
		buf += sizeof( txt->rs[i].port );
	}
	*buf = hex_ascii( pkt->nof_dth );
	buf += sizeof( txt->nof_dth );
	for( i = 0; i < pkt->nof_dth; i++ ){
		ltoh((unsigned long) pkt->dth[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->dth[i].assign_ptn );
		*buf = hex_ascii( pkt->dth[i].drv_type );
		buf += sizeof( txt->dth[i].drv_type );
		*buf = hex_ascii( pkt->dth[i].direction );
		buf += sizeof( txt->dth[i].direction );
		*buf = hex_ascii( pkt->dth[i].level );
		buf += sizeof( txt->dth[i].level );
		*buf = hex_ascii( pkt->dth[i].port );
		buf += sizeof( txt->dth[i].port );
	}
	*buf = hex_ascii( pkt->nof_th );
	buf += sizeof( txt->nof_th );
	for( i = 0; i < pkt->nof_th; i++ ){
		ltoh((unsigned long) pkt->th[i].assign_ptn, (char *)buf );
		buf += sizeof( txt->th[i].assign_ptn );
		*buf = hex_ascii( pkt->th[i].drv_type );
		buf += sizeof( txt->th[i].drv_type );
		*buf = hex_ascii( pkt->th[i].direction );
		buf += sizeof( txt->th[i].direction );
		*buf = hex_ascii( pkt->th[i].level );
		buf += sizeof( txt->th[i].level );
		*buf = hex_ascii( pkt->th[i].port );
		buf += sizeof( txt->th[i].port );
	}
	*buf = hex_ascii( pkt->nof_duc_net );
	buf += sizeof( txt->nof_duc_net );
	for( i = 0; i < pkt->nof_duc_net; i++ ){
		ltoh((unsigned long) pkt->duc_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->duc_assign_ptn[i] );
	}
	*buf = hex_ascii( pkt->mlt_intlk_mode );
	buf += sizeof( txt->mlt_intlk_mode );
	ctoh( pkt->shift_cnt, (char *)buf );
	buf += sizeof( txt->shift_cnt );
	return(buf);

}
