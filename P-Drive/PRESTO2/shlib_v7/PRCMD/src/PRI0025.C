/*======================================================================
 * File name	: pri0025.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0025.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0025.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi009.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi009.c
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
#include	<prcmd\pri0025.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_info							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	各種データ応答テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_info(
	struct fmt_info	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0025s.info_no = htoc((char *)buf->info_no);
	pri0025s.info_desc = htoc((char *)buf->info_desc);
	pri0025s.nof_data = htoc((char *)buf->nof_data);
	p = (BYTE_PTR)buf->data;
	for( i = 0 ; i < pri0025s.nof_data ;i++ ) {
		pri0025s.data[i] = (long)htol((char *)p);
		p += sizeof(buf->data[i]);
	}
	return 0;
}
