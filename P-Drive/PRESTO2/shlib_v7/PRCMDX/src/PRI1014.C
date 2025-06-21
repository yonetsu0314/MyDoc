/*======================================================================
 * File name	: pri1014.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	本体状態情報拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri1014.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri1014.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzi114a.c 1.1 1996/07/26 11:22:25 hiroki Exp
 *log: rzi114a.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri1013.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ata_target						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ata_target)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ata_target(
	struct fmt_ata_target	*txt,
	struct pri1013t		*pkt
	)
{
	char	*buf;
	int	i;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	/*ここから可変長データ処理*/
	buf = (char *)txt->ata_target[0];
	for(i = 0; i < pkt->nof_ch ; i++) {
		ltoh( pkt->ata_target[i], buf);
		buf += sizeof(txt->ata_target[i]);
	}
	return(buf);
}
