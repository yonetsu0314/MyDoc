/*======================================================================
 * File name	: prr4023.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータテキスト解読
 *	ATA SAMPLE SIZE
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr4023.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr4023.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzr423.c 1.2 1996/07/17 17:16:59 hiroki Exp
 *log: rzr423.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr4023.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_sample_size						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ATA SAMPLE SIZE    テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_sample_size(
	struct fmt_sample_size	*buf
	)
{
	prr4023s.sample_size = htoi( (char *)buf->sample_size);
	prr4023s.preset_num  = htoc( (char *)buf->preset_num);
	return 0;
}
