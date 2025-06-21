/*======================================================================
 * File name    : rzr057.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr057.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr057.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr057.cv  1.1  93/11/02 19:27:06  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr057.cv
 * Revision 1.1  93/11/02 19:27:06  ryoji
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
#include	<rzcmd\rzr057.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_afd_sect						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＦＤセクションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_afd_sect(
	struct fmt_afd_sect	*buf
	)
{
	P71		*p;
	int		i;

	rzr057s.ch_id = ascii_hex(buf->ch_id);
	rzr057s.nof_head = htoc((char *)buf->nof_head);
	rzr057s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->sect; i < rzr057s.nof_sect ; i++, p++) {
		rzr057s.sect[i].ctrl_mode = ascii_hex(p->ctrl_mode);
		rzr057s.sect[i].ctrl_sw = (unsigned short)htoi((char *)&p->ctrl_sw);
		rzr057s.sect[i].start_delay = (unsigned short)htoi((char *)&p->start_delay);
		rzr057s.sect[i].df_rf_ratio = htoc((char *)&p->df_rf_ratio);
		rzr057s.sect[i].target_head = htoc((char *)&p->target_head);
		rzr057s.sect[i].amp_center = htoc((char *)&p->amp_center);
		rzr057s.sect[i].amp_max = htoc((char *)&p->amp_max);
		rzr057s.sect[i].amp_min = htoc((char *)&p->amp_min);
		rzr057s.sect[i].amp_range = htoc((char *)&p->amp_range);
	}
	return(0);
}
