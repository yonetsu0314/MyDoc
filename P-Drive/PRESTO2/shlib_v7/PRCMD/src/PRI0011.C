/*======================================================================
 * File name	: pri0011.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0011.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0011.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi011.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi011.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/**/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri0011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_head_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	秤データのテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_head_weight(
	struct fmt_head_weight	*buf
	)
{
	BYTE	*p;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	pri0011s.ch_id = ascii_hex(buf->ch_id);
	pri0011s.flag = ascii_hex(buf->flag);
	pri0011s.min_wh_no = htoc((char *)buf->min_wh_no);
	pri0011s.nof_wh = htoc((char *)buf->nof_wh);
	p = (BYTE_PTR)buf->wh;
	for(i = pri0011s.nof_wh, j = pri0011s.min_wh_no-1 ; i ; i--, j++) {
		pri0011s.wh[j].weight = (short)htoi( (char *)p );
		p += sizeof( buf->wh[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->wh[j].status));
		pri0011s.wh[j].status = (short)htoi((char *)str);
		p += sizeof( buf->wh[j].status );
	}
	pri0011s.nof_bh = htoc((char *)p);
	p += sizeof(buf->nof_bh);
	for(i = pri0011s.nof_bh, j = pri0011s.min_wh_no-1 ; i ; i--, j++) {
		pri0011s.bh[j].weight = (short)htoi( (char *)p );
		p += sizeof( buf->bh[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->bh[j].status));
		pri0011s.bh[j].status = (short)htoi((char *)str);
		p += sizeof( buf->bh[j].status );
	}
	pri0011s.nof_ph = htoc((char *)p);
	p += sizeof(buf->nof_ph);
	for(i = pri0011s.nof_ph, j = pri0011s.min_wh_no-1 ; i ; i--, j++) {
		pri0011s.ph[j].weight = (short)htoi( (char *)p );
		p += sizeof( buf->ph[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->ph[j].status));
		pri0011s.ph[j].status = (short)htoi((char *)str);
		p += sizeof( buf->ph[j].status );
	}
	pri0011s.min_df_no = ascii_hex( *p );
	p += sizeof(buf->min_df_no);
	pri0011s.nof_df = ascii_hex( *p );
	p += sizeof(buf->nof_df);
	for(i = pri0011s.nof_df, j = pri0011s.min_df_no-1 ; i ; i--, j++) {
		pri0011s.df[j].weight = (short)htoi((char *)p );
		p += sizeof( buf->df[j].weight );
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->df[j].status));
		pri0011s.df[j].status = (short)htoi((char *)str);
		p += sizeof( buf->df[j].status );
	}
	return(0);
}
