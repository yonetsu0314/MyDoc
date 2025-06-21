/*======================================================================
 * File name	: prr0022.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0022.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0022.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/09/29  11:04
 * ループする変数の間違いを修正する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzr022.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr022.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0021.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_cf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	CF振幅設定構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_cf_parm(
	struct fmt_cf_parm	*txt,
	struct prr0021t		*pkt
	)
{
	BYTE	*buf;
	int	i,j;

	txt->ctrl = hex_ascii(pkt->ctrl);
	txt->nof_cf = hex_ascii(pkt->nof_cf);

	buf = (BYTE_PTR)txt->cf;
	for( i = 0 ; i < pkt->nof_cf ; i++ ) {
		*buf = hex_ascii( pkt->cf[i].nof_sig );
		buf += sizeof( txt->cf[i].nof_sig );
		for( j = 0 ; j < pkt->cf[i].nof_sig ; j++ ) {
			ctoh( pkt->cf[i].at[j].amp, (char *)buf );
			buf += sizeof(txt->cf[i].at[j].amp);
			ctoh( pkt->cf[i].at[j].time, (char *)buf );
			buf += sizeof(txt->cf[i].at[j].time);
		}
	}
	return(buf);
}
