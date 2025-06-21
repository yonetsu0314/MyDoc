/*======================================================================
 * File name    : rzr423.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（５）テキスト解読
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr523.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr523.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr523.c 1.3 1999/07/12 14:51:28 itami
 *Log: rzr523.c
 * リビジョン 1.3  1999/07/12  14:51:28  itami
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
#include	<rzcmd\rzr523.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_ext_tgt							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張上下限値データ    テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_ext_tgt(
	struct fmt_ext_tgt	*buf
	)
{
	PR5230	*p;
	int	i;

	rzr523s.nof_sect = ascii_hex(buf->nof_sect);

	for(i = 0, p = (PTR)&buf->sect; i < rzr523s.nof_sect ; i++, p++) {
		rzr523s.sect[i].upper_wt = htol((char *)&p->upper_wt);
		rzr523s.sect[i].lower_wt = htol((char *)&p->lower_wt);
		rzr523s.sect[i].upper_cnt = htoi((char *)&p->upper_cnt);
		rzr523s.sect[i].lower_cnt = htoi((char *)&p->lower_cnt);
	}

	return 0;
}
