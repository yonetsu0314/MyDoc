/*======================================================================
 * File name	: pri0015.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	警報発生テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0015.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi015.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi015.c
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
#include	<prcmd\pri0015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_alarm							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	アラームテキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_alarm(
	struct fmt_alarm	*buf
	)
{
	pri0015s.ch_id = ascii_hex(buf->ch_id);
	pri0015s.alarm = htoc((char *)buf->alarm);
	pri0015s.ph_ptn = htol((char *)buf->ph_ptn);
	pri0015s.wh_ptn = htol((char *)buf->wh_ptn);
	pri0015s.bh_ptn = htol((char *)buf->bh_ptn);
	pri0015s.rs_ptn = htoi((char *)buf->rs_ptn);
	pri0015s.dth_ptn = htoi((char *)buf->dth_ptn);
	pri0015s.th_ptn = htoi((char *)buf->th_ptn);
	pri0015s.df_ptn = htoi((char *)buf->df_ptn);
	pri0015s.af_ptn = htol((char *)buf->af_ptn);
	pri0015s.afd = htoc((char *)buf->afd);
	return(0);
}
