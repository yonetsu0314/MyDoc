/*======================================================================
 * File name	: prr0064.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	上下動シュートパラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0064.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0064.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmdx/src/RCS/rzr064.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr064.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom/typedefs.h>		/* Show xlib		*/
#include	<custom/custom.h>		/*	"		*/
#include	<prcmd/prcmd.h>			/* Show Current		*/
#include	<prcmd/prr0063.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_updown_chute						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	上下動シュートパラメータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(updown_shoot_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_updown_chute(
	struct updown_chute_parm	*txt,
	struct prr0063t			*pkt
	)
{
	txt->dev_no = hex_ascii( pkt->dev_no );
	ctoh(pkt->nof_pos, (char *)txt->nof_pos) ;
	ctoh(pkt->err_detect_interval, (char *)txt->err_detect_interval );
	ctoh(pkt->position1, (char *)txt->position1);
	ctoh(pkt->position2, (char *)txt->position2);
	return(++txt);
}
