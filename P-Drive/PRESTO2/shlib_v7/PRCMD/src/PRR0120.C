/*======================================================================
 * File name	: prr0120.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	信号出力先のアサインデータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0120.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0120.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
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
#include	<prcmd\prr0119.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_sig_assign_parm					*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	信号出力先のアサインデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_sig_assign)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_sig_assign_parm(
	struct fmt_sig_assign	*txt,
	struct prr0119t		*pkt
	)
{
	BYTE	*buf;
	int	i, j, k;

	txt->nof_infsig = hex_ascii( pkt->nof_infsig );
	buf = (BYTE_PTR)txt->infsig_assign;
	for( i = 0; i < pkt->nof_infsig; i++ ) {
		ctoh( pkt->infsig_assign[i], (char *)buf );
		buf += sizeof( txt->infsig_assign[i] );
		ctoh( pkt->lowp_sig_assign[i], (char *)buf );
		buf += sizeof( txt->lowp_sig_assign[i] );
	}
	*buf = hex_ascii(pkt->nof_df);
	buf += sizeof( txt->nof_df );
	for( i = 0; i < pkt->nof_df; i++ ) {
		ctoh( pkt->dfsync_sig_assign[i], (char *)buf );
		buf += sizeof( txt->dfsync_sig_assign[i] );
	}
	*buf = hex_ascii( pkt->nof_ch );
	buf += sizeof( txt->nof_ch );
	for( i = 0; i < pkt->nof_ch; i++ ) {
		ctoh( pkt->dumpsig_assign[i], (char *)buf );
		buf += sizeof( txt->dumpsig_assign[i] );
		ctoh( pkt->pr_dumpsig_assign[i], (char *)buf );
		buf += sizeof( txt->pr_dumpsig_assign[i] );
		ctoh( pkt->ng_dumpsig_assign[i], (char *)buf );
		buf += sizeof( txt->ng_dumpsig_assign[i] );
		ctoh( pkt->pr_readysig_assign[i], (char *)buf );
		buf += sizeof( txt->pr_readysig_assign[i] );
		ctoh( pkt->ng_readysig_assign[i], (char *)buf );
		buf += sizeof( txt->ng_readysig_assign[i] );
		ctoh( pkt->pr_each_dpsig_assign[i], (char *)buf );
		buf += sizeof( txt->pr_each_dpsig_assign[i] );
		ctoh( pkt->ng_each_dpsig_assign[i], (char *)buf );
		buf += sizeof( txt->ng_each_dpsig_assign[i] );
		ctoh( pkt->pr_addsig_assign[i], (char *)buf );
		buf += sizeof( txt->pr_addsig_assign[i] );
		ctoh( pkt->ng_addsig_assign[i], (char *)buf );
		buf += sizeof( txt->ng_addsig_assign[i] );
		ctoh( pkt->error_sig_assign[i], (char *)buf );
		buf += sizeof( txt->error_sig_assign[i] );
		ctoh( pkt->control_sig_assign[i], (char *)buf );
		buf += sizeof( txt->control_sig_assign[i] );
		ctoh( pkt->prod_sig_assign[i], (char *)buf );
		buf += sizeof( txt->prod_sig_assign[i] );
		ctoh( pkt->drain_sig_assign[i], (char *)buf );
		buf += sizeof( txt->drain_sig_assign[i] );
		ctoh( pkt->manuzero_sig_assign[i], (char *)buf );
		buf += sizeof( txt->manuzero_sig_assign[i] );
		ctoh( pkt->error_rej_sig_assign[i], (char *)buf );
		buf += sizeof( txt->error_rej_sig_assign[i] );
		ctoh( pkt->lowp_stop_sig_assign[i], (char *)buf );
		buf += sizeof( txt->lowp_stop_sig_assign[i] );
		ctoh( pkt->metal_det_sig_assign[i], (char *)buf );
		buf += sizeof( txt->metal_det_sig_assign[i] );
		*buf = hex_ascii( pkt->pandc[i].nof_sect );
		buf += sizeof( txt->pandc[i].nof_sect );
		for( j = 0; j < pkt->pandc[i].nof_sect; j++ ) {
			*buf = hex_ascii( pkt->pandc[i].sect[j].nof_phead );
			buf += sizeof( txt->pandc[i].sect[j].nof_phead );
			for( k = 0; k < pkt->pandc[i].sect[j].nof_phead; k++ ) {
				ctoh( pkt->pandc[i].sect[j].phead[k].sig_assign, (char *)buf );
				buf += sizeof( txt->pandc[i].sect[j].phead[k].sig_assign );
			}
		}
	}
	return(buf);
}
