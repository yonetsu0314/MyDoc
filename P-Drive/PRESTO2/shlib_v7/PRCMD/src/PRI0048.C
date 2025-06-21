/*======================================================================
 * File name	: pri0048.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ係数テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0048.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0048.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzi204.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzi204.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0047.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_afv_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＦＶ係数データをテキストに変換する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_afv_coeffi(
	struct fmt_afv_coeffi	*txt,
	struct pri0047t		*pkt
	)
{
	ctoh( pkt->wh_no, (char *)txt->wh_no );
	ltoh( (unsigned long)pkt->new_coeffi, (char *)txt->new_coeffi );
	ltoh( (unsigned long)pkt->old_coeffi, (char *)txt->old_coeffi );
	return(++txt);
}
