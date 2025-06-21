/*======================================================================
 * File name	: prr0063.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	上下動シュートパラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0063.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0063.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmdx/src/RCS/rzr063.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr063.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom/typedefs.h>		/* Show xlib		*/
#include	<custom/custom.h>		/*	"		*/
#include	<prcmd/prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd/prr0063.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_updown_chute						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	上下動シュートテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_updown_chute(
	struct updown_chute_parm	*buf
	)
{
	prr0063s.dev_no = ascii_hex( buf->dev_no );
	prr0063s.nof_pos = htoc( (char *)buf->nof_pos );
	prr0063s.err_detect_interval = htoc( (char *)buf->err_detect_interval );
	prr0063s.position1 = htoc( (char *)buf->position1 );
	prr0063s.position2 = htoc( (char *)buf->position2 );
	return 0;
}
