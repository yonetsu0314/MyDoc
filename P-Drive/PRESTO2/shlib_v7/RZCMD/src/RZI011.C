/*======================================================================
 * File name    : rzi011.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi011.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzi011.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:36
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:48
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi011.cv  2.1  93/11/02 19:10:36  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi011.cv
 * Revision 2.1  93/11/02 19:10:36  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzi011.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_head_weight						*/
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
rz_scan_head_weight(
	struct fmt_head_weight	*buf
	)
{
	P9		*p;
	BYTE		*p1;
	int		i, j;

	rzi011s.ch_id = ascii_hex(buf->ch_id);
	rzi011s.min_wh_no = htoc((char *)buf->min_wh_no);
	rzi011s.nof_wh = htoc((char *)buf->nof_wh);
	p = buf->wh;
	for(i = rzi011s.nof_wh, j = rzi011s.min_wh_no-1 ; i ; i--, j++) {
		rzi011s.wh[j].weight = (short)htoi((char *)p->weight);
		rzi011s.wh[j].status = (short)htoc((char *)p->status);
		p++;
	}
	rzi011s.nof_bh = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_bh);
	p = (P9 *)p1;
	for(i = rzi011s.nof_bh, j = rzi011s.min_wh_no-1 ; i ; i--, j++) {
		rzi011s.bh[j].weight = (short)htoi((char *)p->weight);
		rzi011s.bh[j].status = (short)htoc((char *)p->status);
		p++;
	}
	rzi011s.min_df_no = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_df_no);
	p = (P9 *)p1;
	rzi011s.nof_df = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_df);
	p = (P9 *)p1;
	for(i = rzi011s.nof_df, j = rzi011s.min_df_no-1 ; i ; i--, j++) {
		rzi011s.df[j].weight = (short)htoi((char *)p->weight);
		rzi011s.df[j].status = (short)htoc((char *)p->status);
		p++;
	}
	return(0);
}
