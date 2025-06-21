/*======================================================================
 * File name    : rzr033.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr033.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr033.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr033.cv  2.1  93/11/02 19:20:20  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr033.cv
 * Revision 2.1  93/11/02 19:20:20  ryoji
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
#include	<rzcmd\rzr033.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_zero_adj						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	零点調整テキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_zero_adj(
	struct fmt_zero_adj	*buf
	)
{
	rzr033s.ch_id = ascii_hex(buf->ch_id);
	rzr033s.nof_head = htoc((char *)&buf->nof_head);
	rzr033s.adj_head_ptn = htol((char *)&buf->adj_head_ptn);
	rzr033s.adj_df_ptn = htoc((char *) &buf->adj_df_ptn );
	return(0);
}
