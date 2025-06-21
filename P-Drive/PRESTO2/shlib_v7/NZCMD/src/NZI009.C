/*======================================================================
 * File name    : nzi009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi009.c 1.1 1999/05/27 16:51:11 kawamura
 *Log: nzi009.c
 * リビジョン 1.1  1999/05/27  16:51:11  kawamura
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
#include	<nzcmd\nzi009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_info							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各種データ応答テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_info(
	struct fmt_info	*buf
	)
{
	BYTE		*p;
	int		i;

	nzi009s.info_no = htoc((char *)buf->info_no);
	nzi009s.info_desc = htoc((char *)buf->info_desc);
	nzi009s.nof_data = htoc((char *)buf->nof_data);
	for(i = 0, p = (PTR)&buf->data; i < nzi009s.nof_data ;
	    i++, p += sizeof(buf->data[0])) {
		nzi009s.data[i] = htol((char *)p);
	}
	return 0;
}
