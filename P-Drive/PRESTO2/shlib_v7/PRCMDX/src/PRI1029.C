/*======================================================================
 * File name	: pri1029.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助組み合わせ候補重量データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri1029.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri1029.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri1029.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_ref_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助組み合わせ候補重量データを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_ref_weight(
	struct fmt_ref_weight	*buf
	)
{
	BYTE	*p;
	int	i,j;

	pri1029s.ch_id = ascii_hex(buf->ch_id);
	pri1029s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ; i < pri1029s.nof_sect ; i++ ) {
		pri1029s.sect[i].nof_data = ascii_hex(*p);
		p += sizeof(buf->sect[i].nof_data);
		for( j = 0 ; j < pri1029s.sect[i].nof_data ; j++ ) {
			pri1029s.sect[i].weight[j] = htoi((char *)p);
			p += sizeof(buf->sect[i].weight[j]);
		}
	}
	return(0);
}
