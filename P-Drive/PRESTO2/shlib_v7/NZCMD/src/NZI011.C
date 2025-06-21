/*======================================================================
 * File name    : nzi011.c
 * Original	: rzi011.c
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi011.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: nzi011.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  15:22
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/40  09:08
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi011.c 1.1 1999/05/27 16:51:04 kawamura
 *Log: nzi011.c
 * リビジョン 1.1  1999/05/27  16:51:04  kawamura
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
#include	<custom\xmemory.h>		/**/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzi011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_head_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤データのテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_head_weight(
	struct fmt_head_weight_nz	*buf
	)
{
	N9	*p;
	BYTE	*p1;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	nzi011s.ch_id = ascii_hex(buf->ch_id);
	nzi011s.min_wh_no = htoc((char *)buf->min_wh_no);
	nzi011s.nof_wh = htoc((char *)buf->nof_wh);
	p = buf->wh;
	for(i = nzi011s.nof_wh, j = nzi011s.min_wh_no-1 ; i ; i--, j++) {
		nzi011s.wh[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi011s.wh[j].status = (short)htoi((char *)str);
		p++;
	}
	nzi011s.nof_bh = (unsigned char)htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_bh);
	p = (N9 *)p1;
	for(i = nzi011s.nof_bh, j = nzi011s.min_wh_no-1 ; i ; i--, j++) {
		nzi011s.bh[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi011s.bh[j].status = (short)htoi((char *)str);
		p++;
	}
	nzi011s.min_df_no = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_df_no);
	p = (N9 *)p1;
	nzi011s.nof_df = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_df);
	p = (N9 *)p1;
	for(i = nzi011s.nof_df, j = nzi011s.min_df_no-1 ; i ; i--, j++) {
		nzi011s.df[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi011s.df[j].status = (short)htoi((char *)str);
		p++;
	}
	return(0);
}
