/*======================================================================
 * File name    : dzr212.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ヘッドの重心座標テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzr212.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr212.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/19 14:30
 * dzr211sの二重定義を回避する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZR212.Cv  1.3  94/03/18 18:39:14  ryoji
 *Log: P:/LIB/DZCMD/SRC/RCS/DZR212.Cv
 * Revision 1.3  94/03/18 18:39:14  ryoji
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
#include	<dzcmd\dzr211.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	dz_print_afv_coord						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ヘッドの重心座標構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	void *	: txt+sizeof(fmt_afv_coord)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
dz_print_afv_coord(
	struct fmt_afv_coord	*txt,
	struct dzr211t		*pkt
	)
{
	BYTE		*buf;
	int		i;

	buf = (PTR)&txt->pwb_id;
	*buf = hex_ascii( pkt->pwb_id );
	buf += sizeof( txt->pwb_id );
	for(i= 0; i< sizeof(pkt->coord_x)/sizeof(pkt->coord_x[0]); i++) {
		itoh( (unsigned short)pkt->coord_x[i], (char *)buf );
		buf += sizeof( txt->coord_x[0] );
	}
	for(i= 0; i< sizeof(pkt->coord_y)/sizeof(pkt->coord_y[0]); i++) {
		itoh( (unsigned short)pkt->coord_y[i], (char *)buf );
		buf += sizeof( txt->coord_y[0] );
	}
	for(i= 0; i< sizeof(pkt->afv_coeffi)/sizeof(pkt->afv_coeffi[0]); i++) {
		ltoh( (unsigned long)pkt->afv_coeffi[i], (char *)buf );
		buf += sizeof( txt->afv_coeffi[0] );
	}
	return((PTR)buf);
}
