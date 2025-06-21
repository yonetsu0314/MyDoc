/*======================================================================
 * File name    : dzi205.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzi205.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: dzi205.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  13:52
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:18
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZI205.Cv  1.6  94/03/24 10:43:24  kabumoto
 *Log: P:/LIB/DZCMD/SRC/RCS/DZI205.Cv
 * Revision 1.6  94/03/24 10:43:24  kabumoto
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<dzcmd\dzi205.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	dz_scan_diag_data						*/
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
dz_scan_diag_data(
	struct fmt_diag_data	*buf
	)
{
	PI2050		*p;
	BYTE		*p1;
	int		i, j;

	dzi205s.ch_id = ascii_hex(buf->ch_id);
	dzi205s.min_wh_no = htoc((char *)buf->min_wh_no);
	dzi205s.nof_wh = htoc((char *)buf->nof_wh);
	p = buf->wh_3rd;
	for(i = dzi205s.nof_wh, j = dzi205s.min_wh_no-1 ; i ; i--, j++) {
		dzi205s.wh_3rd[j].weight = htoi((char *)p->weight);
		dzi205s.wh_3rd[j].status = htoc((char *)p->status);
		p++;
	}
	for(i = dzi205s.nof_wh, j = dzi205s.min_wh_no-1 ; i ; i--, j++) {
		dzi205s.wh_4th[j].weight = htoi((char *)p->weight);
		dzi205s.wh_4th[j].status = htoc((char *)p->status);
		p++;
	}
	dzi205s.min_df_no = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_df_no);
	p = (PI2050 *)p1;
	dzi205s.nof_df = ascii_hex( *(BYTE_PTR)p );
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_df);
	p = (PI2050 *)p1;
	for(i = dzi205s.nof_df, j = dzi205s.min_df_no-1 ; i ; i--, j++) {
		dzi205s.df[j].weight = htoi((char *)p->weight);
		dzi205s.df[j].status = htoc((char *)p->status);
		p++;
	}
	p1 = (BYTE_PTR)p;
	dzi205s.nof_afv = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_afv);
	for(j = 0 ; j< dzi205s.nof_afv ; j++) {
		dzi205s.afv_out[j] = htoi((char *) p1 );
		p1 += sizeof(buf->afv_out[0]);
	}
	return(0);
}
