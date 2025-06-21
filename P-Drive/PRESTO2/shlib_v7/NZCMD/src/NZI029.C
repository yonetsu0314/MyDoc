/*======================================================================
 * File name    : nzi029.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi029.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi029.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi029.c 1.1 1999/05/27 16:50:44 kawamura
 *Log: nzi029.c
 * リビジョン 1.1  1999/05/27  16:50:44  kawamura
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
#include	<custom\xmemory.h>		/**/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzi029.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	nz_scan_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_pandc_result(
	struct fmt_pandc_result_nz	*buf
	)
{
	NI291	*p0;
	NI292	*p1;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	nzi029s.ch_id = ascii_hex(buf->ch_id);
	nzi029s.nof_head = (unsigned char)htoc((char *)&buf->nof_head);
	nzi029s.nof_sect = ascii_hex(buf->nof_sect);
	for(i=0, p1 = (PTR)&buf->sect; i < nzi029s.nof_sect ; i++) {
		nzi029s.sect[i].nof_phead = ascii_hex(p1->nof_phead);
		for(j=0,p0=(PTR)&p1->phead;j<nzi029s.sect[i].nof_phead;j++,p0++){
			nzi029s.sect[i].phead[j].weight = (short)htoi((char *)&p0->weight);
			memcpy((void *)&str[1], (void *)&p0->wh_status, sizeof(p0->wh_status));
			nzi029s.sect[i].phead[j].wh_status = (short)htoi((char *)str);
			memcpy((void *)&str[1], (void *)&p0->bh_status, sizeof(p0->bh_status));
			nzi029s.sect[i].phead[j].bh_status = (short)htoi((char *)str);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
