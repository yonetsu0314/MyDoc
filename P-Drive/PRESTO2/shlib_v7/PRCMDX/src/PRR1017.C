/*======================================================================
 * File name	: prr1017.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッドの遅れ時間拡張タイミングテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr1017.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr1017.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib_v7/rzcmdx/src/RCS/rzr217.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr217.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr1017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_parent_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各親ヘッドの遅れ時間のテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_parent_tmg(
	struct fmt_parent_tmg	*buf
	)
{
	BYTE	*p;
	int	i,j;

	prr1017s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ; i < prr1017s.nof_sect ; i++ ) {
		prr1017s.sect[i].nof_parent = ascii_hex(*p);
		p += sizeof(buf->sect[i].nof_parent);
		for( j = 0 ; j < prr1017s.sect[i].nof_parent ; j++ ) {
			prr1017s.sect[i].pandc[j].parent_delay = htoc((char *)p);
			p += sizeof(buf->sect[i].pandc[j].parent_delay);
		}
	}
	return(0);
}
