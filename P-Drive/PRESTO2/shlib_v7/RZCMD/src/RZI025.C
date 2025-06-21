/*======================================================================
 * File name    : rzi025.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	平均値制御結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi025.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi025.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi025.cv  1.2  93/11/27 08:51:50  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi025.cv
 * Revision 1.2  93/11/27 08:51:50  ryoji
 * 個数を0.1個単位にする
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
#include	<rzcmd\rzi025.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_mean_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	平均値制御結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_mean_result(
	struct fmt_mean_result	*buf
	)
{
	P81		*p;
	int		i;

	rzi025s.ch_id = ascii_hex(buf->ch_id);
	rzi025s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->sect; i < rzi025s.nof_sect ;
	    i++, p++) {
		rzi025s.sect[i].weight = htol((char *)&p->weight);
		rzi025s.sect[i].count = htol((char *)&p->count);
	}
	rzi025s.ch.weight = htol((char *)&p->weight);
	rzi025s.ch.count = htol((char *)&p->count);
	return(0);
}
