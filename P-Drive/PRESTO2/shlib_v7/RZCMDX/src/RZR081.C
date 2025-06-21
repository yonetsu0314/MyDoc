/*======================================================================
 * File name    : rzr081.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイマーコントロールテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr081.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr081.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr081.c 1.5 1996/09/24 18:58:43 hiroki
 *Log: rzr081.c
 * リビジョン 1.5  1996/09/24  18:58:43  hiroki
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
#include	<rzcmd\rzr081.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_timer_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	タイマー設定内容を構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_timer_parm(
	struct fmt_timer_parm	*buf
	)
{
	PR810	*p;
	int	i;

	rzr081s.ch_id = ascii_hex(buf->ch_id);
	rzr081s.nof_port  = ascii_hex(buf->nof_port);
	p = (PR810 *)buf->timer;
	for(i=0 ; i < rzr081s.nof_port ; i++ , p += 1) {
		rzr081s.timer[i].port  = ascii_hex(p->port);
		rzr081s.timer[i].init  = htoc((char *)p->init);
		rzr081s.timer[i].off   = htoc((char *)p->off);
		rzr081s.timer[i].on    = htoc((char *)p->on);
	}
	return(0);
}
