/*======================================================================
 * File name	: prr2009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＰＨセルデータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr2009.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr2009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr2009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_ph_cell							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＰＨセルデータテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_ph_cell(
	struct fmt_ph_cell	*buf
	)
{
	BYTE	*p;
	int	i,j;

	prr2009s.ph_span_adj = ascii_hex(buf->ph_span_adj);
	prr2009s.nof_ch = ascii_hex(buf->nof_ch);
	p = (BYTE_PTR)buf->ch;
	for(i = 0 ; i < prr2009s.nof_ch ; i++ ) {
		prr2009s.ch[i].stable_time = htoi( (char *)p );
		p += sizeof(buf->ch[i].stable_time);
		prr2009s.ch[i].zero_interval = htoc( (char *)p );
		p += sizeof(buf->ch[i].zero_interval);
		prr2009s.ch[i].nof_sect = ascii_hex( *p );
		p += sizeof(buf->ch[i].nof_sect);
		for(j = 0 ; j < (int)prr2009s.ch[i].nof_sect ; j++ ) {
			prr2009s.ch[i].sect[j].empty_weight = htoi( (char *)p );
			p += sizeof(buf->ch[i].sect[j].empty_weight);
			prr2009s.ch[i].sect[j].stick_weight = htoi( (char *)p );
			p += sizeof(buf->ch[i].sect[j].stick_weight);
			prr2009s.ch[i].sect[j].ph_weight_check = ascii_hex( *p );
			p += sizeof(buf->ch[i].sect[j].ph_weight_check);
		}
	}
	return(0);
}
