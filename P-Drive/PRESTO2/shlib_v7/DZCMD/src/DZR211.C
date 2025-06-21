/*======================================================================
 * File name    : dzr211.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ヘッドの重心座標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr211.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr211.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZR211.Cv  1.3  94/03/18 18:35:54  ryoji
 *Log: P:/LIB/DZCMD/SRC/RCS/DZR211.Cv
 * Revision 1.3  94/03/18 18:35:54  ryoji
 * 重心座標関数バグ修正
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<dzcmd\dzr211.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	dz_scan_afv_coord						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ヘッドの重心座標テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
dz_scan_afv_coord(
	struct fmt_afv_coord	*buf
	)
{
	BYTE		*p;
	WORD		*wp;
	DWORD		*dp;
	int		i;

	p = (PTR)&buf->pwb_id;
	dzr211s.pwb_id = ascii_hex( buf->pwb_id );
	p += sizeof( buf->pwb_id );
	wp = (WORD *)&dzr211s.coord_x[0];
	for(i= sizeof(dzr211s.coord_x)/sizeof(dzr211s.coord_x[0]); i ; i--) {
		*(wp++) = (WORD)htoi((char *) p );
		p += sizeof( buf->coord_x[0] );
	}
	wp = (WORD *)&dzr211s.coord_y[0];
	for(i= sizeof(dzr211s.coord_y)/sizeof(dzr211s.coord_y[0]); i ; i--) {
		*(wp++) =(WORD)htoi((char *) p );
		p += sizeof( buf->coord_y[0] );
	}
	dp = (DWORD *)&dzr211s.afv_coeffi[0];
	for(i= sizeof(dzr211s.afv_coeffi)/sizeof(dzr211s.afv_coeffi[0]); i ; i--) {
		*(dp++) = htol( (char *)p );
		p += sizeof( buf->afv_coeffi[0] );
	}
	return(0);
}
