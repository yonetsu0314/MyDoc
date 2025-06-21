/*======================================================================
 * File name    : rzi009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi009.cv  2.2  94/04/12 17:44:14  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi009.cv
 * Revision 2.2  94/04/12 17:44:14  kabumoto
 * ワーニング除去
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
#include	<rzcmd\rzi009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_prgno							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	プログラム番号テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_prgno(
	struct fmt_prgno	*buf
	)
{
	rzi009s.dev_id = ascii_hex(buf->dev_id);
	rzi009s.head_no = htoc((char *)buf->head_no);
	strncpy((char *)&rzi009s.rom_no, (char *)&buf->rom_no,
		sizeof(buf->rom_no));
	rzi009s.ver_prefix = htoc((char *)buf->ver_prefix);
	rzi009s.ver_suffix = htoc((char *)buf->ver_suffix);
	return(0);
}
