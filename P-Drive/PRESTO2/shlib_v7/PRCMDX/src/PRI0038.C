/*======================================================================
 * File name	: pri0038.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡモニター情報テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0038.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0038.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi038.c 1.3 1996/07/15 15:24:50 hiroki Exp
 *log: rzi038.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0037.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_dca_monitor						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＤＣＡモニター情報をテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_monitor)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_dca_monitor(
	struct fmt_dca_monitor	*txt,
	struct pri0037t		*pkt
	)
{
	itoh( (unsigned short)pkt->dca_sum_cnt, (char *)txt->dca_sum_cnt );
	ltoh( pkt->bf_cbwt, (char *)txt->bf_cbwt );
	ltoh( pkt->af_cbwt, (char *)txt->af_cbwt );
	return(++txt);
}
