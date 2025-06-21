/*======================================================================
 * File name    : rzi013.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	状態テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi013.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi013.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi013.cv  2.1  93/11/02 19:12:42  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi013.cv
 * Revision 2.1  93/11/02 19:12:42  ryoji
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
#include	<rzcmd\rzi013.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_condition						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_condition(
	struct fmt_condition	*buf
	)
{
	P91		*p;
	int		i;

	rzi013s.nof_ch = ascii_hex(buf->nof_ch);
	for(i = 0, p = (PTR)&buf->ch; i < rzi013s.nof_ch ; i++, p++) {
		rzi013s.ch[i].condition = ascii_hex(p->condition);
		rzi013s.ch[i].power_stat = ascii_hex(p->power_stat);
		rzi013s.ch[i].infeed_stat = ascii_hex(p->infeed_stat);
	}
	return(0);
}
