/*======================================================================
 * File name	: pri0003.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0003.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0003.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi003.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi003.c
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
#include	<prcmd\pri0003.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_cb_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算結果テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_cb_result(
	struct fmt_cb_result	*buf
	)
{
	BYTE	*p;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	pri0003s.ch_id = ascii_hex(buf->ch_id);
	pri0003s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ; i < pri0003s.nof_sect ; i++ ) {
		pri0003s.sect[i].flag = htoc( (char *)p );
		p += sizeof( buf->sect[i].flag );
		pri0003s.sect[i].weight = (long)htol((char *)p );
		p += sizeof( buf->sect[i].weight );
		pri0003s.sect[i].count = (short)htoi((char *)p );
		p += sizeof( buf->sect[i].count );
	}

	pri0003s.ch.flag = htoc( (char *)p );
	p += sizeof( buf->ch.flag );
	pri0003s.ch.weight = (long)htol((char *)p );
	p += sizeof( buf->ch.weight );
	pri0003s.ch.count = (short)htoi((char *)p );
	p += sizeof( buf->ch.count );
	pri0003s.multi_dump_seq_no = htoc( (char *)p );
	p += sizeof(buf->multi_dump_seq_no);
	pri0003s.slv_targ_wt = (long)htol( (char *)p );
	p += sizeof(buf->slv_targ_wt);
	pri0003s.slv_targ_cnt = (short)htoi( (char *)p );
	p += sizeof(buf->slv_targ_cnt);
	pri0003s.min_wh_no = htoc( (char *)p );
	p += sizeof(buf->min_wh_no);

	pri0003s.nof_wh = htoc( (char *)p );
	p += sizeof(buf->nof_wh);
	for(i = pri0003s.nof_wh, j = pri0003s.min_wh_no-1 ; i ; i--, j++) {
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->wh_status[j]));
		pri0003s.wh_status[j] = htoi((char *)str);
		p += sizeof(buf->wh_status[j]);
	}

	pri0003s.nof_bh = htoc((char *)p);
	p += sizeof(buf->nof_bh);
	for(i = pri0003s.nof_bh, j = pri0003s.min_wh_no-1 ; i ; i--, j++) {
		memcpy((PTR)&str[1], (PTR)p, sizeof(buf->bh_status[j]));
		pri0003s.bh_status[j] = htoi((char *)str);
		p += sizeof(buf->bh_status[j]);
	}
	return(0);
}
