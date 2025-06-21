/*======================================================================
 * File name	: pri0001.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0001.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0001.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi001.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi001.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<custom\xmemory.h>		/**/
#define		STORAGE
#include	<prcmd\pri0001.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_diag_data						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤診断テキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_diag_data(
	struct fmt_diag_data	*buf
	)
{
	BYTE	*p;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	pri0001s.ch_id = ascii_hex(buf->ch_id);
	pri0001s.min_wh_no = htoc((char *)buf->min_wh_no);
	pri0001s.nof_wh = htoc((char *)buf->nof_wh);
	p = (BYTE_PTR)buf->wh_3rd;
	for(i = pri0001s.nof_wh, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.wh_3rd[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->wh_3rd[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->wh_3rd[j].status));
		pri0001s.wh_3rd[j].status = (short)htoi((char *)str);
		p += sizeof( buf->wh_3rd[j].status );
	}
	for(i = pri0001s.nof_wh, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.wh_4th[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->wh_4th[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->wh_4th[j].status));
		pri0001s.wh_4th[j].status = (short)htoi((char *)str);
		p += sizeof( buf->wh_4th[j].status );
	}
	for(i = pri0001s.nof_wh, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.wh_5th[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->wh_5th[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->wh_5th[j].status));
		pri0001s.wh_5th[j].status = (short)htoi((char *)str);
		p += sizeof( buf->wh_5th[j].status );
	}
	for(i = pri0001s.nof_ph, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.ph_3rd[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->ph_3rd[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->ph_3rd[j].status));
		pri0001s.ph_3rd[j].status = (short)htoi((char *)str);
		p += sizeof( buf->ph_3rd[j].status );
	}
	for(i = pri0001s.nof_ph, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.ph_4th[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->ph_4th[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->ph_4th[j].status));
		pri0001s.ph_4th[j].status = (short)htoi((char *)str);
		p += sizeof( buf->ph_4th[j].status );
	}
	for(i = pri0001s.nof_ph, j = pri0001s.min_wh_no-1 ; i ; i--, j++) {
		pri0001s.ph_5th[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->ph_5th[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->ph_5th[j].status));
		pri0001s.ph_5th[j].status = (short)htoi((char *)str);
		p += sizeof( buf->ph_5th[j].status );
	}
	pri0001s.min_df_no = ascii_hex( *p );
	p += sizeof(buf->min_df_no);
	pri0001s.nof_df = ascii_hex( *p );
	p += sizeof(buf->nof_df);
	for(i = pri0001s.nof_df, j = pri0001s.min_df_no-1 ; i ; i--, j++) {
		pri0001s.df[j].weight = (short)htoi((char *)p);
		p += sizeof( buf->df[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->df[j].status));
		pri0001s.df[j].status = (short)htoi((char *)str);
		p += sizeof( buf->df[j].status );
	}
	pri0001s.nof_afv = htoc((char *)p);
	p += sizeof(buf->nof_afv);
	for(i = 0 ; i < pri0001s.nof_afv ; i++) {
		pri0001s.afv_out[i] = htoi( (char *)p );
		p += sizeof(buf->afv_out[i]);
	}
	return(0);
}
