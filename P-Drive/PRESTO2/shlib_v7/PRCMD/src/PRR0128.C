/*======================================================================
 * File name	: prr0128.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数設定データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0128.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0128.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
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
#include	<prcmd\prr0127.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_hop_drive_set						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動回数設定データの構造体をテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_hop_drive_set)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_hop_drive_set(
	struct fmt_hop_drive_set	*txt,
	struct prr0127t			*pkt
	)
{
	txt->hopper_id = hex_ascii( pkt->hopper_id );
	ctoh(pkt->hopper_no, (char *)txt->hopper_no);
	ltoh(pkt->nof_times, (char *)txt->nof_times);
	return(++txt);
}
