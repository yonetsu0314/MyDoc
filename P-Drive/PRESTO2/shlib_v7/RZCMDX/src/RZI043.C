/*======================================================================
 * File name    : rzi043.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi043.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi043.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzi043.c 1.1 1997/01/08 09:00:01 chimura
 *Log: rzi043.c
 * リビジョン 1.1  1997/01/08  09:00:01  chimura
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzi043.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_span_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	スパン係数応答テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_span_coeffi(
	struct fmt_span_coeffi	*buf
	)
{
	BYTE	*p;
	WORD	*wp;
	int	i;

	rzi043s.pwb_id = ascii_hex(buf->pwb_id);
	rzi043s.date = htol((char *)&buf->date);
	rzi043s.time = htol((char *)&buf->time);
	p = (BYTE_PTR)&buf->span_coeffi;
	wp = &rzi043s.span_coeffi[0];
	for(i= sizeof(rzi043s.span_coeffi)/sizeof(rzi043s.span_coeffi[0]);
								 i ; i--) {
		*(wp++) = htoi( (char *)p );
		p += sizeof( buf->span_coeffi[0] );
	}
	return(0);
}
