/*======================================================================
 * File name	: pri0014.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	状態テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0014.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0014.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi014a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi014a.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0013.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_condition						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_condition)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_condition(
	struct fmt_condition	*txt,
	struct pri0013t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	buf = (BYTE_PTR)txt->ch;
	for(i = 0 ; i < pkt->nof_ch ; i++ ) {
		*buf = hex_ascii(pkt->ch[i].condition);
		buf += sizeof(txt->ch[i].condition);
		*buf = hex_ascii(pkt->ch[i].power_stat);
		buf += sizeof(txt->ch[i].power_stat);
		*buf = hex_ascii(pkt->ch[i].infeed_stat);
		buf += sizeof(txt->ch[i].infeed_stat);
	}
	ctoh(pkt->df_infeed_stat, (char *)buf);
	buf += sizeof(txt->df_infeed_stat);
	return buf;
}
