/*======================================================================
 * File name	: prr0119.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	信号出力先のアサインデータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0119.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0119.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
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
#include	<prcmd\prr0119.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_sig_assign_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	信号出力先のアサインデータを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_sig_assign_parm(
	struct fmt_sig_assign	*buf
	)
{
	BYTE	*p;
	int	i, j, k;

	p = (BYTE_PTR)buf->infsig_assign;
	prr0119s.nof_infsig = ascii_hex(buf->nof_infsig);
	for( i = 0; i < prr0119s.nof_infsig; i++ ) {
		prr0119s.infsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->infsig_assign[i] );
		prr0119s.lowp_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->lowp_sig_assign[i] );
	}
	prr0119s.nof_df = ascii_hex( *p );
	p += sizeof( buf->nof_df );
	for( i = 0; i < prr0119s.nof_df; i++ ) {
		prr0119s.dfsync_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->dfsync_sig_assign[i] );
	}
	prr0119s.nof_ch = ascii_hex( *p );
	p += sizeof( buf->nof_ch );
	for( i = 0; i < prr0119s.nof_ch; i++ ) {
		prr0119s.dumpsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->dumpsig_assign[i] );
		prr0119s.pr_dumpsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->pr_dumpsig_assign[i] );
		prr0119s.ng_dumpsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->ng_dumpsig_assign[i] );
		prr0119s.pr_readysig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->pr_readysig_assign[i] );
		prr0119s.ng_readysig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->ng_readysig_assign[i] );
		prr0119s.pr_each_dpsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->pr_each_dpsig_assign[i] );
		prr0119s.ng_each_dpsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->ng_each_dpsig_assign[i] );
		prr0119s.pr_addsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->pr_addsig_assign[i] );
		prr0119s.ng_addsig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->ng_addsig_assign[i] );
		prr0119s.error_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->error_sig_assign[i] );
		prr0119s.control_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->control_sig_assign[i] );
		prr0119s.prod_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->prod_sig_assign[i] );
		prr0119s.drain_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->drain_sig_assign[i] );
		prr0119s.manuzero_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->manuzero_sig_assign[i] );
		prr0119s.error_rej_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->error_rej_sig_assign[i] );
		prr0119s.lowp_stop_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->lowp_stop_sig_assign[i] );
		prr0119s.metal_det_sig_assign[i] = htoc( (char *)p );
		p += sizeof( buf->metal_det_sig_assign[i] );
		prr0119s.pandc[i].nof_sect = ascii_hex( *p );
		p += sizeof( buf->pandc[i].nof_sect );
		for( j = 0; j < prr0119s.pandc[i].nof_sect; j++ ) {
			prr0119s.pandc[i].sect[j].nof_phead = ascii_hex( *p );
			p += sizeof( buf->pandc[i].sect[j].nof_phead );
			for( k = 0; k < prr0119s.pandc[i].sect[j].nof_phead; k++ ) {
				prr0119s.pandc[i].sect[j].phead[k].sig_assign = htoc( (char *)p );
				p += sizeof( buf->pandc[i].sect[j].phead[k].sig_assign );
			}
		}
	}
	return(0);
}
