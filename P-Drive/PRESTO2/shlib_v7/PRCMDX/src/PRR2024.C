/*======================================================================
 * File name	: prr2024.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（２）テキスト作成
 *	特殊アベコン
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr2024.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr2024.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib_v7/rzcmdx/src/RCS/rzr224.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr224.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr2023.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_avg_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	特殊アベコンの構造体をテキストに変換する			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prec_dump)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_avg_parm(
	struct fmt_avg_parm 	*txt,
	struct prr2023t		*pkt
	)
{
	ctoh(pkt->avg_cycle, (char *)&txt->avg_cycle);
	ctoh(pkt->avg_ratio, (char *)&txt->avg_ratio);
	itoh(pkt->avg_reset_count, (char *)&txt->avg_reset_count);
	return(++txt);
}
