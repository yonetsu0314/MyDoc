/*======================================================================
 * File name	: prr1006.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給振り分け装置ゲート位置拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr1006.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr1006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2006, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr1005.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_gate_position						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	供給振り分け装置ゲート位置テキストを構造体にセーブする。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_gate_position)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_gate_position(
	struct fmt_gate_position	*txt,
	struct prr1005t			*pkt
	)
{
	int	i;
	char	*buf;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	for(i = 0; i < pkt->nof_ch ; i++) {
		txt->gate_position[i] = hex_ascii(pkt->gate_position[i]);
	}
	buf = (char *)&txt->gate_position[0];
	buf += i;
	return( buf);
}
