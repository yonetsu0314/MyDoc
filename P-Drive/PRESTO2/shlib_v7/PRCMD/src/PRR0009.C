/*======================================================================
 * File name	: prr0009.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0009.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr009.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr009.c
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
#include	<prcmd\prr0009.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_wei_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤計量特性テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_wei_spec(
	struct fmt_wei_spec	*buf
	)
{
	BYTE	*p;
	int	i,j;

	prr0009s.nof_adc = ascii_hex( buf->nof_adc );

	p = (BYTE_PTR)buf->adc;
	for(i = 0 ; i < (int)prr0009s.nof_adc ; i++ ) {
		prr0009s.adc[i].stable_count = htoc( (char *)p );
		p += sizeof( buf->adc[i].stable_count );
		prr0009s.adc[i].zero_limit_cnt = htoi( (char *)p );
		p += sizeof( buf->adc[i].zero_limit_cnt );
		prr0009s.adc[i].stable_cmp_cnt = ascii_hex( *p );
		p += sizeof( buf->adc[i].stable_cmp_cnt );
		prr0009s.adc[i].range = ascii_hex( *p );
		p += sizeof( buf->adc[i].range );
		prr0009s.adc[i].smpl_interval = htoc( (char *)p );
		p += sizeof( buf->adc[i].smpl_interval );
		prr0009s.adc[i].iir_reset_cnt = htoc( (char *)p );
		p += sizeof( buf->adc[i].iir_reset_cnt );
		prr0009s.adc[i].afv_mode = ascii_hex( *p );
		p += sizeof( buf->adc[i].afv_mode );
		prr0009s.adc[i].filter = ascii_hex( *p );
		p += sizeof( buf->adc[i].filter );
		prr0009s.adc[i].span_wt = ascii_hex( *p );
		p += sizeof( buf->adc[i].span_wt );
		prr0009s.adc[i].cell_type = ascii_hex( *p );
		p += sizeof( buf->adc[i].cell_type );
		prr0009s.adc[i].nof_stage = ascii_hex( *p );
		p += sizeof( buf->adc[i].nof_stage );
		for(j = 0 ; j < (int)prr0009s.adc[i].nof_stage ; j++ ) {
			prr0009s.adc[i].dadfip[j].thin = htoi( (char *)p );
			p += sizeof(buf->adc[i].dadfip[j].thin);
			prr0009s.adc[i].dadfip[j].dig = htoi( (char *)p );
			p += sizeof(buf->adc[i].dadfip[j].dig);
			prr0009s.adc[i].dadfip[j].tap = htoi( (char *)p );
			p += sizeof(buf->adc[i].dadfip[j].tap);
			prr0009s.adc[i].dadfip[j].coef = htoi( (char *)p );
			p += sizeof(buf->adc[i].dadfip[j].coef);
		}
		prr0009s.adc[i].afv_wh_limit = htoi( (char *)p );
		p += sizeof( buf->adc[i].afv_wh_limit );
		prr0009s.adc[i].afv_afv_limit = htoi( (char *)p );
		p += sizeof( buf->adc[i].afv_afv_limit );
		prr0009s.adc[i].afv_onoff_limit = htoi( (char *)p );
		p += sizeof( buf->adc[i].afv_onoff_limit );
	}
	prr0009s.nof_ch = ascii_hex( *p );
	p += sizeof( buf->nof_ch );
	for(i = 0 ; i < (int)prr0009s.nof_ch ; i++ ) {
		prr0009s.ch[i].stable_time = htoi( (char *)p );
		p += sizeof(buf->ch[i].stable_time);
		prr0009s.ch[i].zero_interval = htoc( (char *)p );
		p += sizeof(buf->ch[i].zero_interval);
		prr0009s.ch[i].nof_sect = ascii_hex( *p );
		p += sizeof(buf->ch[i].nof_sect);
		for(j = 0 ; j < (int)prr0009s.ch[i].nof_sect ; j++ ) {
			prr0009s.ch[i].sect[j].empty_weight = htoi( (char *)p );
			p += sizeof(buf->ch[i].sect[j].empty_weight);
			prr0009s.ch[i].sect[j].stick_weight = htoi( (char *)p );
			p += sizeof(buf->ch[i].sect[j].stick_weight);
		}
	}
	return(0);
}
