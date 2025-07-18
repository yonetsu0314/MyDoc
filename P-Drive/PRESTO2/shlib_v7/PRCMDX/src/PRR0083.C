/*======================================================================
 * File name    : prr0083.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ参照（要求）仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr0083.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0083.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr083.c 1.1 1970/01/01
 *Log: rzr083.c
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
#include	<prcmd\prr0083.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_shift_mon						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	シフトデータ参照（要求）仕様を構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int		
pr_scan_shift_mon( 
	struct fmt_shift_mon	*buf
	)
{
	prr0083s.ch_id 		= ascii_hex( buf->ch_id );
	prr0083s.req_type 	= ascii_hex( buf->req_type );
	prr0083s.data_page 	= htoc( (char *)buf->data_page );
	return( 0 );
}
