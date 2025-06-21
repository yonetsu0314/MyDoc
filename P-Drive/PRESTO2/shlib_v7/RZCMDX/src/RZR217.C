/*======================================================================
 * File name    : rzr217.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張タイミングテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr217.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr217.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr217.c 1.2 1997/01/08 19:43:27 hiroe
 *Log: rzr217.c
 * リビジョン 1.2  1997/01/08  19:43:27  hiroe
 * テキストが可変長になるようにプログラム修正をする
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
#include	<rzcmd\rzr217.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_parent_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各親ヘッドの遅れ時間のテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_parent_tmg(
	struct fmt_parent_tmg	*buf
	)
{
	P42	*p1;
	P43	*p2;
	int	i, j;

	rzr217s.nof_sect = ascii_hex( buf->nof_sect );
	for(i=0, p1=(PTR)&buf->sect; i<rzr217s.nof_sect; i++) {
	   rzr217s.sect[i].nof_parent = ascii_hex(p1->nof_parent);
	   for(j=0,p2=(PTR)&p1->pandc; j<rzr217s.sect[i].nof_parent; j++,p2++){
		rzr217s.sect[i].pandc[j].parent_delay = htoc((char *)p2->parent_delay);
	   }
	   p1 = (PTR)p2;
	}
	return(0);
}
