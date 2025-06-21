/*======================================================================
 * File name	: prr0076.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡパラメータコマンドテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr0076.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0076.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/lib/rzcmdx/src/RCS/rzr076.c 1.4 1996/07/12 19:27:54 hiroki Exp
 *log: rzr076.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0075.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_dca_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＤＣＡパラメータコマンドをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *pr_print_dca_parm(
	struct fmt_dca_parm	*txt,
	struct prr0075t		*pkt
	)
{
	txt->dca_on_off = hex_ascii( pkt->dca_on_off );
	itoh( pkt->dca_cycle, (char *)txt->dca_cycle );
	ctoh( pkt->dca_owf, (char *)txt->dca_owf );
	ctoh( pkt->dca_sdm, (char *)txt->dca_sdm );
	return(++txt);
}
