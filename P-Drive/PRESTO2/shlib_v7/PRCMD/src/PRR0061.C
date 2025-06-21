/*======================================================================
 * File name	: prr0061.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ヘッドの重心座標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0061.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0061.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzr211.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzr211.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0061.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_afv_coord						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ヘッドの重心座標テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_afv_coord(
	struct fmt_afv_coord	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0061s.pwb_id = ascii_hex( buf->pwb_id );
	p = (BYTE_PTR)buf->coord_x[0];
	for( i = 0 ; i < sizeof(prr0061s.coord_x)/sizeof(prr0061s.coord_x[0]); i++ ) {
		prr0061s.coord_x[i] = (short)htoi((char *) p );
		p += sizeof( buf->coord_x[i] );
	}
	for( i = 0 ; i < sizeof(prr0061s.coord_y)/sizeof(prr0061s.coord_y[0]); i++ ) {
		prr0061s.coord_y[i] = (short)htoi((char *) p );
		p += sizeof( buf->coord_y[i] );
	}
	for( i = 0 ; i < sizeof(prr0061s.afv_coeffi)/sizeof(prr0061s.afv_coeffi[0]); i++ ) {
		prr0061s.afv_coeffi[i] = (long)htol((char *) p );
		p += sizeof( buf->afv_coeffi[i] );
	}
	return(0);
}
