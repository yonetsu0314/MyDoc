/*======================================================================
 * File name	: prr0122.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	リレー駆動要求データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0122.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0122.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0121.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ryout_check						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	リレー駆動要求データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ryout_check)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ryout_check(
	struct fmt_ryout_check	*txt,
	struct prr0121t		*pkt
	)
{
	txt->brd_no = hex_ascii( pkt->brd_no );
	ctoh(pkt->drive_ptn, (char *)txt->drive_ptn);
	ctoh(pkt->drive_delay, (char *)txt->drive_delay);
	ctoh(pkt->drive_time, (char *)txt->drive_time);
	return(++txt);
}
