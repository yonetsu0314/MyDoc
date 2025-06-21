/*======================================================================
 * File name	: prr0062.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ヘッドの重心座標テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0062.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0062.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzr212.c 1.2 1970/01/01 00:00:00 chimura Exp
 *$log: dzr212.c
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
#include	<prcmd\prr0061.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_afv_coord						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ヘッドの重心座標構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	void *	: txt+sizeof(fmt_afv_coord)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_afv_coord(
	struct fmt_afv_coord	*txt,
	struct prr0061t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->pwb_id = hex_ascii( pkt->pwb_id );
	buf = (BYTE_PTR)txt->coord_x;

	for( i = 0; i < sizeof(pkt->coord_x)/sizeof(pkt->coord_x[0]); i++) {
		itoh( (unsigned short)pkt->coord_x[i], (char *)buf );
		buf += sizeof( txt->coord_x[i] );
	}
	for( i = 0; i < sizeof(pkt->coord_y)/sizeof(pkt->coord_y[0]); i++) {
		itoh( (unsigned short)pkt->coord_y[i], (char *)buf );
		buf += sizeof( txt->coord_y[i] );
	}
	for( i = 0; i < sizeof(pkt->afv_coeffi)/sizeof(pkt->afv_coeffi[0]); i++) {
		ltoh( (unsigned long)pkt->afv_coeffi[i], (char *)buf );
		buf += sizeof( txt->afv_coeffi[i] );
	}
	return(buf);
}
