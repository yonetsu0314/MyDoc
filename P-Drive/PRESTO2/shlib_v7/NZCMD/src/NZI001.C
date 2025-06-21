/*======================================================================
 * File name    : nzi001.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi001.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: nzi001.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  15:25
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:06
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi001.c 1.1 1999/05/27 16:51:26 kawamura
 *Log: nzi001.c
 * リビジョン 1.1  1999/05/27  16:51:26  kawamura
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
#include	<nzcmd\nzi001.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_diag_data						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	秤診断テキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_diag_data(
	struct fmt_diag_data_nz	*buf
	)
{
	NI0010		*p;
	BYTE		*p1;
	int		i, j;
	char		str[4];

	str[0] = (char)'0';
	nzi001s.ch_id = ascii_hex(buf->ch_id);
	nzi001s.min_wh_no = htoc((char *)buf->min_wh_no);
	nzi001s.nof_wh = htoc((char *)buf->nof_wh);
	p = buf->wh_3rd;
	for(i = nzi001s.nof_wh, j = nzi001s.min_wh_no-1 ; i ; i--, j++) {
		nzi001s.wh_3rd[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi001s.wh_3rd[j].status = (short)htoi((char *)str);
		p++;
	}
	for(i = nzi001s.nof_wh, j = nzi001s.min_wh_no-1 ; i ; i--, j++) {
		nzi001s.wh_4th[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi001s.wh_4th[j].status = (short)htoi((char *)str);
		p++;
	}
	nzi001s.min_df_no = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_df_no);
	p = (NI0010 *)p1;
	nzi001s.nof_df = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_df);
	p = (NI0010 *)p1;
	for(i = nzi001s.nof_df, j = nzi001s.min_df_no-1 ; i ; i--, j++) {
		nzi001s.df[j].weight = (short)htoi((char *)p->weight);
		memcpy((void *)&str[1], (void *)p->status, sizeof(p->status));
		nzi001s.df[j].status = (short)htoi((char *)str);
		p++;
	}
	p1 = (BYTE_PTR)p;
	nzi001s.nof_afv = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_afv);
	for(j = 0 ; j< nzi001s.nof_afv ; j++) {
		nzi001s.afv_out[j] = (unsigned short)htoi( (char *)p1 );
		p1 += sizeof(buf->afv_out[0]);
	}
	return(0);
}
