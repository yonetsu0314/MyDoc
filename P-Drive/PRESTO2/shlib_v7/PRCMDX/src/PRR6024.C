/*======================================================================
 * File name	: prr6024.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	セミオート機データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr6024.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr6024.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/12/06  15:34
 * ＢＨ非同期供給のON/OFFを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr6023.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_semi_auto						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	セミオート機の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_semi_auto)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_semi_auto(
	struct fmt_semi_auto	*txt,
	struct prr6023t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->dump_dir = hex_ascii(pkt->dump_dir);
	txt->conv_rev = hex_ascii(pkt->conv_rev);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i = 0 ;i < pkt->nof_sect ;i++ ) {
		*buf = hex_ascii( pkt->sect[i].wh_async_supply );
		buf += sizeof( txt->sect[i].wh_async_supply );
		*buf = hex_ascii( pkt->sect[i].only_bh_cal );
		buf += sizeof( txt->sect[i].only_bh_cal );
		*buf = hex_ascii( pkt->sect[i].bh_async_supply );
		buf += sizeof( txt->sect[i].bh_async_supply );
	}
	return(buf);
}
