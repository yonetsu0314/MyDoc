/*======================================================================
 * File name    : nzi007.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi007.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi007.c 1.1 1999/05/27 16:49:54 kawamura
 *Log: nzi007.c
 * リビジョン 1.1  1999/05/27  16:49:54  kawamura
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
#include	<nzcmd\nzi007.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_archive							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	重量波形データ応答テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_archive(
	struct fmt_archive	*buf
	)
{
	NI0070		*p;
	int		i;

	nzi007s.wh_no = htoc((char *)buf->wh_no);
	nzi007s.index = (unsigned short)htoi((char *)buf->index);
	nzi007s.nof_data = htoc((char *)buf->nof_data);
	for(i = 0, p = (PTR)&buf->data; i < nzi007s.nof_data ;
	    i++, p++) {
		nzi007s.data[i].weight = (short)htoi((char *)p->weight);
		nzi007s.data[i].status = ascii_hex(p->status);
	}
	return 0;
}
