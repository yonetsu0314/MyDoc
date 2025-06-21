/*======================================================================
 * File name    : rzi111.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張秤データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi111.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi111.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi111.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzi111.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzi111.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_ph_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張秤データのテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_ph_weight(
	struct fmt_ph_weight	*buf
	)
{
	PR111	*p;
	int	i, j;

	rzi111s.ch_id = ascii_hex(buf->ch_id);
	rzi111s.min_ph_no = htoc((char *)buf->min_ph_no);
	rzi111s.nof_ph = htoc((char *)buf->nof_ph);
	p = buf->ph;
	for(i = rzi111s.nof_ph, j = rzi111s.min_ph_no-1 ; i ; i--, j++) {
		rzi111s.ph[j].weight = htoi((char *)p->weight);
		rzi111s.ph[j].status = htoc((char *)p->status);
		p++;
	}
	return(0);
}
