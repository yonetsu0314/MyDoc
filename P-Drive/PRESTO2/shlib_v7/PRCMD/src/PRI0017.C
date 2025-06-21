/*======================================================================
 * File name	: pri0017.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0017.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0017.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi017a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi017a.c
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
#include	<prcmd\pri0017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_error							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	エラーテキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_error(
	struct fmt_error	*buf
	)
{
	pri0017s.ch_id = ascii_hex(buf->ch_id);
	pri0017s.error = htoc((char *)buf->error);
	pri0017s.ph_ptn = htol((char *)buf->ph_ptn);
	pri0017s.wh_ptn = htol((char *)buf->wh_ptn);
	pri0017s.bh_ptn = htol((char *)buf->bh_ptn);
	pri0017s.rs_ptn = htoi((char *)buf->rs_ptn);
	pri0017s.dth_ptn = htoi((char *)buf->dth_ptn);
	pri0017s.th_ptn = htoi((char *)buf->th_ptn);
	pri0017s.df_ptn = htoc((char *)buf->df_ptn);
	pri0017s.af_ptn = htol((char *)buf->af_ptn);
	pri0017s.duc_inf = ascii_hex(buf->duc_inf);
	pri0017s.exc_inf = ascii_hex(buf->exc_inf);
	return 0;
}
