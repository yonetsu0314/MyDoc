/*======================================================================
 * File name	: pri0009.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi009a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi009a.c
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
#include	<prcmd\pri0009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_prgno							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	プログラム番号テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_prgno(
	struct fmt_prgno	*buf
	)
{
	pri0009s.brd_id = ascii_hex(buf->brd_id);
	pri0009s.dev_no = ascii_hex(buf->dev_no);
	pri0009s.node_id = htoc((char *)buf->node_id);
	pri0009s.len = htoc((char *)buf->len);
	strncpy((char *)&pri0009s.rom_id, (char *)buf->rom_id, (size_t)pri0009s.len);
	return 0;
}
