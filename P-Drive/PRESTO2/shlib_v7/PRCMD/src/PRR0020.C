/*======================================================================
 * File name	: prr0020.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給量テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0020.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0020.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzr020.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr020.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0019.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_rfdf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	供給量設定構造体のデータをテキストに変換する。			*/
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
pr_print_rfdf_parm(
	struct fmt_rfdf_parm	*txt,
	struct prr0019t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->backup = hex_ascii( pkt->backup );
	ctoh( pkt->nof_rf , (char *)txt->nof_rf );

	buf = (BYTE_PTR)txt->rf;
	for( i = 0; i < (int)pkt->nof_rf; i++ ) {
		ctoh(pkt->rf[i].amp, (char *)buf);
		buf += sizeof(txt->rf[i].amp);
		ctoh(pkt->rf[i].time, (char *)buf);
		buf += sizeof(txt->rf[i].time);
	}

	*buf = hex_ascii(pkt->nof_df);
	buf += sizeof(txt->nof_df);
	for( i = 0; i < (int)pkt->nof_df; i++ ) {
		ctoh(pkt->df[i].amp, (char *)buf);
		buf += sizeof(txt->df[i].amp);
		ctoh(pkt->df[i].time, (char *)buf);
		buf += sizeof(txt->df[i].time);
	}

	ctoh( pkt->nof_af , (char *)buf );
	buf += sizeof(txt->nof_af);
	for( i = 0; i < (int)pkt->nof_af; i++ ) {
		ctoh(pkt->af[i].amp, (char *)buf);
		buf += sizeof(txt->af[i].amp);
		ctoh(pkt->af[i].time, (char *)buf);
		buf += sizeof(txt->af[i].time);
	}
	return(buf);
}
