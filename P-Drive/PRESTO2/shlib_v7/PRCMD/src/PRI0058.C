/*======================================================================
 * File name	: pri0058.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0058.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0058.c $
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
#include	<prcmd\pri0057.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_hop_drive_no						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動回数応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_hop_drive_no)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_hop_drive_no(
	struct fmt_hop_drive_no	*txt,
	struct pri0057t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->hopper_id = hex_ascii( pkt->hopper_id );
	ctoh(pkt->nof_hopper, (char *)txt->nof_hopper);
	buf = (BYTE_PTR)txt->nof_times[0];
	for(i = 0 ; i < pkt->nof_hopper; i++) {
		ltoh(pkt->nof_times[i], (char *)buf);
		buf += sizeof( txt->nof_times[i] );
	}
	return(buf);
}
