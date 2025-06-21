/*======================================================================
 * File name    : prr0091.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *　マルチ連動パラメータ1テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr0091.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0091.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr091.c 1.3 1997/12/08 21:39:27
 *Log: rzr091.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0091.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/* 	pr_scan_mlt_parm1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動パラメータ1のテキストを構造体にセーブする。    　　	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_mlt_parm1 (
	struct fmt_mlt_parm1	*buf
	)
{
	prr0091s.ch_id		= ascii_hex( buf->ch_id);
	prr0091s.neighbor_node	= htoc( (char *)buf->neighbor_node);
	prr0091s.shift_cnt	= htoc( (char *)buf->shift_cnt);
	prr0091s.retry_time    	= htoc( (char *)buf->retry_time);
	prr0091s.retry_span    	= htoc( (char *)buf->retry_span);
	prr0091s.retry_cnt    	= htoc( (char *)buf->retry_cnt);
	prr0091s.master_slave    = htoc( (char *)buf->master_slave);
	prr0091s.ch_status    	= htoc( (char *)buf->ch_status);
	*((unsigned short *)&prr0091s.dsw) = htoi( (char *)buf->dsw);

	return(0);
}
