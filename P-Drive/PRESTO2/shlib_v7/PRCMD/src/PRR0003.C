/*======================================================================
 * File name	: prr0003.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0003.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0003.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr003.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr003.c
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
#include	<prcmd\prr0003.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_mch_construct						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	計量機構成仕様を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_mch_construct(
	struct fmt_mch_construct	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0003s.head_construct = ascii_hex( buf->head_construct );
	prr0003s.nof_head = htoc( (char *)buf->nof_head );
	prr0003s.model_id = ascii_hex( buf->model_id );
	prr0003s.nof_df = ascii_hex( buf->nof_df );

	p = (BYTE_PTR)buf->df_assign_ptn[0];
	for( i = 0; i < (int)prr0003s.nof_df; i++ ) {
		prr0003s.df_assign_ptn[i] = htol( (char *)p );
		p += sizeof( buf->df_assign_ptn[i] );
	}
	prr0003s.nof_af = htoc( (char *)p );
	p += sizeof( buf->nof_af );
	for( i = 0; i < (int)prr0003s.nof_af; i++ ) {
		prr0003s.af_assign_ptn[i] = htol( (char *)p );
		p += sizeof( buf->af_assign_ptn[i] );
	}
	prr0003s.nof_rs = htoc( (char *)p );
	p += sizeof( buf->nof_rs );
	for( i = 0; i < (int)prr0003s.nof_rs; i++ ) {
		prr0003s.rs[i].assign_ptn = htol( (char *)p );
		p += sizeof( buf->rs[i].assign_ptn );
		prr0003s.rs[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->rs[i].drv_type );
		prr0003s.rs[i].direction = ascii_hex( *p );
		p += sizeof( buf->rs[i].direction );
		prr0003s.rs[i].level = ascii_hex( *p );
		p += sizeof( buf->rs[i].level );
		prr0003s.rs[i].port = htoc( (char *)p );
		p += sizeof( buf->rs[i].port );
	}
	prr0003s.nof_dth = htoc( (char *)p );
	p += sizeof( buf->nof_dth );
	for( i = 0; i < (int)prr0003s.nof_dth; i++ ) {
		prr0003s.dth[i].assign_ptn = htol( (char *)p );
		p += sizeof( buf->dth[i].assign_ptn );
		prr0003s.dth[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->dth[i].drv_type );
		prr0003s.dth[i].direction = ascii_hex( *p );
		p += sizeof( buf->dth[i].direction );
		prr0003s.dth[i].level = ascii_hex( *p );
		p += sizeof( buf->dth[i].level );
		prr0003s.dth[i].port = htoc( (char *)p );
		p += sizeof( buf->dth[i].port );
	}
	prr0003s.nof_th = htoc( (char *)p );
	p += sizeof( buf->nof_th );
	for( i = 0; i < (int)prr0003s.nof_th; i++ ) {
		prr0003s.th[i].assign_ptn = htol( (char *)p );
		p += sizeof( buf->th[i].assign_ptn );
		prr0003s.th[i].drv_type = ascii_hex( *p );
		p += sizeof( buf->th[i].drv_type );
		prr0003s.th[i].direction = ascii_hex( *p );
		p += sizeof( buf->th[i].direction );
		prr0003s.th[i].level = ascii_hex( *p );
		p += sizeof( buf->th[i].level );
		prr0003s.th[i].port = htoc( (char *)p );
		p += sizeof( buf->th[i].port );
	}
	prr0003s.nof_duc_net = ascii_hex( *p );
	p += sizeof( buf->nof_duc_net );
	for( i = 0; i < prr0003s.nof_duc_net; i++ ){
		prr0003s.duc_assign_ptn[i] = htol((char *)p );
		p += sizeof( buf->duc_assign_ptn[i] );
	}
	return( 0 );
}
