/*======================================================================
 * File name	: prr2006.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	4-20mA制御用拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr2006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr2006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
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
#include	<prcmd\prr2005.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_max_df_weight						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	4-20mA制御用拡張パラメータテキストを構造体にセーブする。	*/
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
pr_print_max_df_weight(
	struct fmt_max_df_weight	*txt,
	struct prr2005t			*pkt
	)
{
	BYTE	*buf;
	int	i;

	buf = (BYTE_PTR)txt->nof_sig;
	ctoh( pkt->nof_sig, (char *)buf );
	buf += sizeof(txt->nof_sig);
	for( i = 0; i < pkt->nof_sig; i++ ) {
		itoh( (unsigned short)pkt->max_df_weight[i], (char *)buf );
		buf += sizeof( txt->max_df_weight[i] );
	}
	return( buf);
}
