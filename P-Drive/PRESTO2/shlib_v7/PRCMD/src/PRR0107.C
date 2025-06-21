/*======================================================================
 * File name	: prr0107.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0107.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0107.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr051.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzr051.c
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
#include	<prcmd\prr0107.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_res_detect						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ共振周期検出開始を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
pr_scan_res_detect(
	struct fmt_resonance_detect	*buf
	)
{
	prr0107s.rf_detect_ptn = htol((char *)buf->rf_detect_ptn);
	prr0107s.df_detect_ptn = htol((char *)buf->df_detect_ptn);
	prr0107s.af_detect_ptn = htol((char *)buf->af_detect_ptn);
	prr0107s.cf_detect_ptn = htol((char *)buf->cf_detect_ptn);
	return( 0 );
}
