/*======================================================================
 * File name    : rzr003.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr003.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr003.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr003.cv  2.1  93/11/02 20:29:10  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr003.cv
 * Revision 2.1  93/11/02 20:29:10  ryoji
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
#include	<rzcmd\rzr003.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_mch_construct						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量機構成仕様を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int		
rz_scan_mch_construct( 
	struct fmt_mch_construct	*buf
	)
{
	unsigned char *p;
	register int	i;

	rzr003s.head_construct = ascii_hex( buf->head_construct );
	rzr003s.nof_head 	= htoc((char *) buf->nof_head );
	rzr003s.model_id 	= ascii_hex( buf->model_id );
	rzr003s.nof_df 	= ascii_hex( buf->nof_df );
	/*ここから可変長データ処理*/
	p = (BYTE_PTR)buf->df_assign_ptn[0];
	for( i = 0; i < rzr003s.nof_df; i++ ){
		rzr003s.df_assign_ptn[i] = htol((char *) p );
		p += sizeof( buf->df_assign_ptn[i] );
	}
	rzr003s.nof_rs	= ascii_hex( *p );
	p += sizeof( buf->nof_rs );
	for( i = 0; i < rzr003s.nof_rs; i++ ){
		rzr003s.rs[i].assign_ptn = htol((char *) p );
		p += sizeof( buf->rs[i].assign_ptn );
		rzr003s.rs[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->rs[i].drv_type );
		rzr003s.rs[i].direction = ascii_hex( *p );
		p += sizeof( buf->rs[i].direction );
		rzr003s.rs[i].level = ascii_hex( *p );
		p += sizeof( buf->rs[i].level );
		rzr003s.rs[i].port = ascii_hex( *p );
		p += sizeof( buf->rs[i].port );
	}
	rzr003s.nof_dth	= ascii_hex( *p );
	p += sizeof( buf->nof_dth );
	for( i = 0; i < rzr003s.nof_dth; i++ ){
		rzr003s.dth[i].assign_ptn = htol((char *) p );
		p += sizeof( buf->dth[i].assign_ptn );
		rzr003s.dth[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->dth[i].drv_type );
		rzr003s.dth[i].direction = ascii_hex( *p );
		p += sizeof( buf->dth[i].direction );
		rzr003s.dth[i].level = ascii_hex( *p );
		p += sizeof( buf->dth[i].level );
		rzr003s.dth[i].port = ascii_hex( *p );
		p += sizeof( buf->dth[i].port );
	}
	rzr003s.nof_th	= ascii_hex( *p );
	p += sizeof( buf->nof_th );
	for( i = 0; i < rzr003s.nof_th; i++ ){
		rzr003s.th[i].assign_ptn = htol((char *) p );
		p += sizeof( buf->th[i].assign_ptn );
		rzr003s.th[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->th[i].drv_type );
		rzr003s.th[i].direction = ascii_hex( *p );
		p += sizeof( buf->th[i].direction );
		rzr003s.th[i].level = ascii_hex( *p );
		p += sizeof( buf->th[i].level );
		rzr003s.th[i].port = ascii_hex( *p );
		p += sizeof( buf->th[i].port );
	}
	rzr003s.nof_duc_net	= ascii_hex( *p );
	p += sizeof( buf->nof_duc_net );
	for( i = 0; i < rzr003s.nof_duc_net; i++ ){
		rzr003s.duc_assign_ptn[i] = htol((char *) p );
		p += sizeof( buf->duc_assign_ptn[i] );
	}
	rzr003s.mlt_intlk_mode = ascii_hex( *p );
	p += sizeof( buf->mlt_intlk_mode );
	rzr003s.shift_cnt = htoc((char *) p );
	return( 0 );
}
