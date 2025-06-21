/*======================================================================
 * File name    : rzr055.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤヘッドデータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr055.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr055.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr055.cv  1.1  93/11/02 19:32:06  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr055.cv
 * Revision 1.1  93/11/02 19:32:06  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr055.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_afd_head						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＦＤヘッドデータのテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_afd_head(
	struct fmt_afd_head	*buf
	)
{
	P92		*p;
	int		i, j;

	rzr055s.ch_id = ascii_hex(buf->ch_id);
	rzr055s.min_wh_no = htoc((char *)buf->min_wh_no);
	rzr055s.nof_wh = htoc((char *)buf->nof_wh);
	p = buf->wh;
	for(i = rzr055s.nof_wh, j = rzr055s.min_wh_no-1 ; i ; i--, j++) {
		rzr055s.wh[j].trg = (unsigned short)htoi((char *)p->trg);
		rzr055s.wh[j].std = (unsigned short)htoi((char *)p->std);
		p++;
	}
	return(0);
}
