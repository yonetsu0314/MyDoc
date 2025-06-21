/*======================================================================
 * File name	: prr0051.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0051.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0051.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr051.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr051.c
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
#include	<prcmd\prr0051.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	テスト駆動開始テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_tstdrv_run(
	struct fmt_tstdrv_run	*buf
	)
{
	prr0051s.ch_id = ascii_hex(buf->ch_id);
	prr0051s.nof_head = htoc((char *)buf->nof_head);
	prr0051s.cf_ptn = htoc((char *)buf->cf_ptn);
	prr0051s.df_ptn = htoc((char *)buf->df_ptn);
	prr0051s.af_ptn = htol((char *)buf->af_ptn);
	prr0051s.rf_ptn = htol((char *)buf->rf_ptn);
	prr0051s.sht_ptn = htol((char *)buf->sht_ptn);
	prr0051s.ph_ptn = htol((char *)buf->ph_ptn);
	prr0051s.wh_ptn = htol((char *)buf->wh_ptn);
	prr0051s.wh2_ptn = htol((char *)buf->wh2_ptn);
	prr0051s.bh_ptn = htol((char *)buf->bh_ptn);
	prr0051s.rs_ptn = htoi((char *)buf->rs_ptn);
	prr0051s.dth_ptn = htoi((char *)buf->dth_ptn);
	prr0051s.th_ptn = htoi((char *)buf->th_ptn);
	prr0051s.spa_ptn = htol((char *)buf->spa_ptn);
	prr0051s.drv_cyc = ascii_hex( buf->drv_cyc );
	prr0051s.sht_flg = ascii_hex( buf->sht_flg);
	prr0051s.spa_flg = ascii_hex( buf->spa_flg);
	return(0);
}
