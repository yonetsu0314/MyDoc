/*======================================================================
 * File name	: pri0005.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	単重テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi005.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi005.c
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
#include	<prcmd\pri0005.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_piece_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	単重自動更新結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_piece_weight(
	struct fmt_piece_weight	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0005s.ch_id = ascii_hex(buf->ch_id);
	pri0005s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->weight;
	for(i = 0 ; i < pri0005s.nof_sect ; i++ ) {
		pri0005s.weight[i] = htoi((char *)p);
		p += sizeof(buf->weight[i]);
	}
	return(0);
}
