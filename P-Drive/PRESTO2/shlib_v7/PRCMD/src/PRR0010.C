/*======================================================================
 * File name	: prr0010.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0010.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr010.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr010.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_wei_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤計量特性構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_wei_spec(
	struct fmt_wei_spec	*txt,
	struct prr0009t		*pkt
	)
{
	BYTE	*buf;
	int	i,j;

	txt->nof_adc = hex_ascii( pkt->nof_adc );
	buf = (BYTE_PTR)txt->adc[0].stable_count;

	for( i = 0; i < (int)pkt->nof_adc; i++ ) {
		ctoh( pkt->adc[i].stable_count, (char *)buf );
		buf += sizeof( txt->adc[i].stable_count );
		itoh( pkt->adc[i].zero_limit_cnt, (char *)buf );
		buf += sizeof( txt->adc[i].zero_limit_cnt );
		*buf = hex_ascii( pkt->adc[i].stable_cmp_cnt );
		buf += sizeof( txt->adc[i].stable_cmp_cnt );
		*buf = hex_ascii( pkt->adc[i].range );
		buf += sizeof( txt->adc[i].range );
		ctoh( pkt->adc[i].smpl_interval, (char *)buf );
		buf += sizeof( txt->adc[i].smpl_interval );
		ctoh( pkt->adc[i].iir_reset_cnt, (char *)buf );
		buf += sizeof( txt->adc[i].iir_reset_cnt );
		*buf = hex_ascii( pkt->adc[i].afv_mode );
		buf += sizeof( txt->adc[i].afv_mode );
		*buf = hex_ascii( pkt->adc[i].filter );
		buf += sizeof( txt->adc[i].filter );
		*buf = hex_ascii( pkt->adc[i].span_wt );
		buf += sizeof( txt->adc[i].span_wt );
		*buf = hex_ascii( pkt->adc[i].cell_type );
		buf += sizeof( txt->adc[i].cell_type );
		*buf = hex_ascii( pkt->adc[i].nof_stage );
		buf += sizeof( txt->adc[i].nof_stage );
		for( j = 0; j < (int)pkt->adc[i].nof_stage; j++ ) {
			itoh(pkt->adc[i].dadfip[j].thin, (char *)buf );
			buf += sizeof( txt->adc[i].dadfip[j].thin );
			itoh(pkt->adc[i].dadfip[j].dig, (char *)buf );
			buf += sizeof( txt->adc[i].dadfip[j].dig );
			itoh(pkt->adc[i].dadfip[j].tap, (char *)buf );
			buf += sizeof( txt->adc[i].dadfip[j].tap );
			itoh(pkt->adc[i].dadfip[j].coef, (char *)buf );
			buf += sizeof( txt->adc[i].dadfip[j].coef );
		}
		itoh( pkt->adc[i].afv_wh_limit, (char *)buf );
		buf += sizeof( txt->adc[i].afv_wh_limit );
		itoh( pkt->adc[i].afv_afv_limit, (char *)buf );
		buf += sizeof( txt->adc[i].afv_afv_limit );
		itoh( pkt->adc[i].afv_onoff_limit, (char *)buf );
		buf += sizeof( txt->adc[i].afv_onoff_limit );
	}
	*buf = hex_ascii( pkt->nof_ch );
	buf += sizeof(txt->nof_ch);
	for( i = 0; i < (int)pkt->nof_ch; i++ ) {
		itoh( pkt->ch[i].stable_time, (char *)buf );
		buf += sizeof( txt->ch[i].stable_time );
		ctoh( pkt->ch[i].zero_interval, (char *)buf );
		buf += sizeof( txt->ch[i].zero_interval );
		*buf = hex_ascii( pkt->ch[i].nof_sect );
		buf += sizeof( txt->ch[i].nof_sect );
		for( j = 0; j < (int)pkt->ch[i].nof_sect; j++ ) {
			itoh( pkt->ch[i].sect[j].empty_weight, (char *)buf );
			buf += sizeof( txt->ch[i].sect[j].empty_weight );
			itoh( pkt->ch[i].sect[j].stick_weight, (char *)buf );
			buf += sizeof( txt->ch[i].sect[j].stick_weight );
		}
	}
	return(buf);
}
