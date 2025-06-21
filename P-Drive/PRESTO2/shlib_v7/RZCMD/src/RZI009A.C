/*======================================================================
 * File name    : rzi009a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用プログラム番号テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi009a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi009a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi009a.c 1.1 1995/11/21 09:29:20 hiroe
 *Log: rzi009a.c
 * リビジョン 1.1  1995/11/21  09:29:20  hiroe
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
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzi009a.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_prgno_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用プログラム番号テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_prgno_v3(
	struct fmt_prgno_v3	*buf
	)
{
	rzi009s.brd_id = ascii_hex(buf->brd_id);
	rzi009s.dev_no = ascii_hex(buf->dev_no);
	rzi009s.node_id = htoc((char *)buf->node_id);
	rzi009s.len = htoc((char *)buf->len);
	strncpy((char *)&rzi009s.rom_id, (char *)&buf->rom_id, (size_t)rzi009s.len);

	return 0;
}
