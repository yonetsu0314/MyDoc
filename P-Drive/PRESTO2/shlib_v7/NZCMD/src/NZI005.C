/*======================================================================
 * File name    : nzi005.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ロジック波形データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi005.c 1.1 1999/05/27 16:50:32 kawamura
 *Log: nzi005.c
 * リビジョン 1.1  1999/05/27  16:50:32  kawamura
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
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzi005.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_logic							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ロジック波形データ応答テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_logic(
	struct fmt_logic	*buf
	)
{
	BYTE		*p;
	int		i;

	nzi005s.unit_no = ascii_hex(buf->unit_no);
	nzi005s.nof_data = htoc((char *)buf->nof_data);
	for(i = 0, p = (PTR)&buf->data; i < nzi005s.nof_data ;
	    i++, p += sizeof(buf->data[0])) {
		nzi005s.data[i] = htoi((char *)p);
	}
	return 0;
}
