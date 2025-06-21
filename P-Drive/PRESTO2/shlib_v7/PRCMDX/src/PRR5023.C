/*======================================================================
 * File name    : prr5023.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（５）テキスト解読
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr5023.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr5023.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr523.c 1.1 1970/01/01
 *Log: rzr523.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr5023.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_ext_tgt							*/
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
pr_scan_ext_tgt(
	struct fmt_ext_tgt	*buf
	)
{
	PR50230	*p;
	int	i;

	prr5023s.nof_sect = ascii_hex(buf->nof_sect);

	for(i = 0, p = (PTR)&buf->sect; i < prr5023s.nof_sect ; i++, p++) {
		prr5023s.sect[i].upper_wt = htol((char *)&p->upper_wt);
		prr5023s.sect[i].lower_wt = htol((char *)&p->lower_wt);
		prr5023s.sect[i].upper_cnt = htoi((char *)&p->upper_cnt);
		prr5023s.sect[i].lower_cnt = htoi((char *)&p->lower_cnt);
	}

	return 0;
}
