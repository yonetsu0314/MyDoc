/*======================================================================
 * File name    : rzi013a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用状態テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi013a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi013a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi013a.c 1.1 1995/11/21 09:21:46 hiroe
 *Log: rzi013a.c
 * リビジョン 1.1  1995/11/21  09:21:46  hiroe
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
#include	<rzcmd\rzi013a.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_condition_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用コンディションテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_condition_v3(
	struct fmt_condition_v3	*buf
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
	rzi013s.df_infeed_stat = htoc((char *)(char *)p);
	return 0;
}
