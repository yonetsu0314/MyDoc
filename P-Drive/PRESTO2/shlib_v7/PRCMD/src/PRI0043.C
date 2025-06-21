/*======================================================================
 * File name	: pri0043.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0043.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0043.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmdx/src/rcs/rzi043.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi043.c
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
#include	<prcmd\pri0043.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_span_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	スパン係数応答テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_span_coeffi(
	struct fmt_span_coeffi	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0043s.pwb_id = ascii_hex(buf->pwb_id);
	pri0043s.range = ascii_hex(buf->range);
	pri0043s.date = htol((char *)buf->date);
	pri0043s.time = htol((char *)buf->time);
	p = (BYTE_PTR)buf->span_coeffi;
	for( i = 0 ; i < sizeof(pri0043s.span_coeffi)/sizeof(pri0043s.span_coeffi[0]) ; i++ ) {
		pri0043s.span_coeffi[i] = htoi( (char *)p );
		p += sizeof( buf->span_coeffi[i] );
	}
	return(0);
}
