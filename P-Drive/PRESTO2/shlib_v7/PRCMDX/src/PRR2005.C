/*======================================================================
 * File name	: prr2005.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	4-20mA制御用拡張パラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr2005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr2005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2006, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr2005.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_max_df_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	4-20mA制御用拡張パラメータテキストを構造体にセーブする。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_max_df_weight(
	struct fmt_max_df_weight	*buf
	)
{
	BYTE	*p;
	int	i;

	prr2005s.nof_sig = htoc( (char *)buf->nof_sig );
	p = (BYTE_PTR)buf->max_df_weight[0];
	for( i = 0; i < prr2005s.nof_sig; i++ ) {
		prr2005s.max_df_weight[i] = (short)htoi( (char *)p );
		p += sizeof( buf->max_df_weight[i] );
	}
	return(0);
}
