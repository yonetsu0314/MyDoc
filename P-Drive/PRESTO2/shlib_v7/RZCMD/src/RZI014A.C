/*======================================================================
 * File name    : rzi014a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用状態テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi014a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi014a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi014a.c 1.1 1995/11/21 09:23:14 hiroe
 *Log: rzi014a.c
 * リビジョン 1.1  1995/11/21  09:23:14  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzi013a.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_condition_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用コンディションテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_condition_v3)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_condition_v3(
	struct fmt_condition_v3	*txt,
	struct rzi013t		*pkt
	)
{
	BYTE		*buf;
	RZI0130T	*p;
	int		i;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	buf = (PTR)txt->ch;
	for(i = 0, p = (PTR)&pkt->ch; i < pkt->nof_ch ; i++, p++) {
		*buf = hex_ascii(p->condition);
		buf += sizeof(txt->ch[i].condition);
		*buf = hex_ascii(p->power_stat);
		buf += sizeof(txt->ch[i].power_stat);
		*buf = hex_ascii(p->infeed_stat);
		buf += sizeof(txt->ch[i].infeed_stat);
	}
	ctoh(pkt->df_infeed_stat, (char *)buf);
	buf += sizeof(txt->df_infeed_stat);
	return buf;
}
