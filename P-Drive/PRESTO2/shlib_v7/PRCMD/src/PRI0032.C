/*======================================================================
 * File name	: pri0032.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0032.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0032.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi032.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi032.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0031.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_cf_rslt						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	CF制御結果のデータをテキストに変換する。			*/
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
pr_print_cf_rslt(
	struct fmt_cf_rslt	*txt,
	struct pri0031t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->seq_no = hex_ascii(pkt->seq_no);
	txt->nof_cf = hex_ascii(pkt->nof_cf);
	buf = (BYTE_PTR)txt->cf;
	for( i = 0 ; i < pkt->nof_cf ; i++ ) {
		*buf = hex_ascii(pkt->cf[i].nof_sig);
		buf += sizeof(txt->cf[i].nof_sig);
		for( j = 0 ; j < pkt->cf[i].nof_sig; j++ ) {
			ctoh(pkt->cf[i].at[j].amp, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].amp);
			ctoh(pkt->cf[i].at[j].time, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].time);
		}
		ctoh(pkt->cf[i].sig_stat, (char *)buf);
		buf += sizeof(txt->cf[i].sig_stat);
		itoh((unsigned short)pkt->cf[i].df_wt, (char *)buf);
		buf += sizeof(txt->cf[i].df_wt);
	}
	return(buf);
}
