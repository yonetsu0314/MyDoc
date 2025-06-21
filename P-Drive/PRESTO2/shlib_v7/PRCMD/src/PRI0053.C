/*======================================================================
 * File name	: pri0053.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0053.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0053.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi007.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi007.c
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
#include	<prcmd\pri0053.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_archive							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	重量波形データ応答テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_archive(
	struct fmt_archive	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0053s.hopper_id = ascii_hex(buf->hopper_id);
	pri0053s.wh_no = htoc((char *)buf->wh_no);
	pri0053s.index = htoi((char *)buf->index);
	pri0053s.nof_data = htoc((char *)buf->nof_data);
	p = (BYTE_PTR)buf->data;
	for( i = 0 ; i < pri0053s.nof_data ; i++ ) {
		pri0053s.data[i].weight = (short)htoi((char *)p );
		p += sizeof(buf->data[i].weight);
		pri0053s.data[i].status = ascii_hex( *p );
		p += sizeof(buf->data[i].status);
	}
	return 0;
}
