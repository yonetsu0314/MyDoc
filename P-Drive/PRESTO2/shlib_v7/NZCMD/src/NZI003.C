/*======================================================================
 * File name    : nzi003.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi003.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: nzi003.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  15:23
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:07
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi003.c 1.1 1999/05/27 16:51:19 kawamura
 *Log: nzi003.c
 * リビジョン 1.1  1999/05/27  16:51:19  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<custom\xmemory.h>		/**/
#define		STORAGE
#include	<nzcmd\nzi003.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_cb_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算結果テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_cb_result(
	struct fmt_cb_result_nz	*buf
	)
{
	N8	*p;
	BYTE	*p1;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	nzi003s.ch_id = ascii_hex(buf->ch_id);
	nzi003s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->sect; i < nzi003s.nof_sect ;
	    i++, p++) {
		nzi003s.sect[i].flag = ascii_hex(p->flag);
		nzi003s.sect[i].weight = htol((char *)&p->weight);
		nzi003s.sect[i].count = (short)htoi((char *)&p->count);
	}
	nzi003s.ch.flag = ascii_hex(p->flag);
	nzi003s.ch.weight = htol((char *)&p->weight);
	nzi003s.ch.count = (short)htoi((char *)&p->count);
	p++;
	nzi003s.slv_targ_wt = htol((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->slv_targ_wt);
	p = (N8 *)p1;
	nzi003s.slv_targ_cnt = (short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->slv_targ_cnt);
	p = (N8 *)p1;
	nzi003s.min_wh_no = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_wh_no);
	p = (N8 *)p1;
	nzi003s.nof_wh = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_wh);
	p = (N8 *)p1;
	for(i = nzi003s.nof_wh, j = nzi003s.min_wh_no-1 ; i ; i--, j++) {
		memcpy((void *)&str[1], (void *)p, sizeof(buf->wh_status[0]));
		nzi003s.wh_status[j] = htoi((char *)str);
		p1 = (BYTE_PTR)p;
		p1 += sizeof(buf->wh_status[0]);
		p = (N8 *)p1;
	}
	nzi003s.nof_bh = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_bh);
	p = (N8 *)p1;
	for(i = nzi003s.nof_bh, j = nzi003s.min_wh_no-1 ; i ; i--, j++) {
		memcpy((void *)&str[1], (void *)p, sizeof(buf->bh_status[0]));
		nzi003s.bh_status[j] = htoi((char *)str);
		p1 = (BYTE_PTR)p;
		p1 += sizeof(buf->bh_status[0]);
		p = (N8 *)p1;
	}
	return(0);
}
