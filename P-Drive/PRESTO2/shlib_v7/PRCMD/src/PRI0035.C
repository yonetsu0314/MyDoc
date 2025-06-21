/*======================================================================
 * File name	: pri0035.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0035.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0035.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi035.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi035.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show prcmd		*/
#define		STORAGE
#include	<prcmd\pri0035.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_netmap							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ネットワーク参加状態テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_netmap(
	struct fmt_netmap	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0035s.brd_id = ascii_hex(buf->brd_id);
	pri0035s.dev_no = htoc((char *)buf->dev_no);
	p = (BYTE_PTR)buf->net_map[0];
	for( i = 0; i < sizeof(pri0035s.net_map) ; i++ ) {
		pri0035s.net_map[i] = htoc( (char *)p );
		p += sizeof(buf->net_map[i]);
	}
	pri0035s.recon = htoi((char *)buf->recon);
	return(0);
}
