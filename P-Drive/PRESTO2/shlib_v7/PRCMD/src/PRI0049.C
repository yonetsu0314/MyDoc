/*======================================================================
 * File name	: pri0049.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ロジック波形データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0049.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0049.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi005.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi005.c
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
#define		STORAGE
#include	<prcmd\pri0049.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_logic							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ロジック波形データ応答テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_logic(
	struct fmt_logic	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0049s.unit_no = ascii_hex(buf->unit_no);
	pri0049s.nof_data = htoc((char *)buf->nof_data);
	p = (BYTE_PTR)buf->data;
	for( i = 0 ; i < pri0049s.nof_data ; i++ ) {
		pri0049s.data[i] = htoi((char *)p);
		p += sizeof(buf->data[i]);
	}
	return 0;
}
