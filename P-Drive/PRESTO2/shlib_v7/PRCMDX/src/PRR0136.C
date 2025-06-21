/*======================================================================
 * File name	: prr0136.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	特殊仕様データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/rcs/prr0136.c 1.1 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0136.c $
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0135.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_sp_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	特殊仕様データの構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_sp_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_sp_parm(
	struct fmt_sp_parm	*txt,
	struct prr0135t		*pkt
	)
{
	BYTE		*buf;
	PRR01350T	*p;
	int		i, k;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_parm, (char *)txt->nof_parm);
	buf = (PTR)txt->sp_parm;
	for(i = 0, p = (PRR01350T *)pkt->sp_parm; i < pkt->nof_parm ; i++, p++) {
		ctoh(p->spec_no, (char *)buf);
		buf += sizeof(txt->sp_parm[i].spec_no);
		*buf = hex_ascii(p->nof_data);
		buf += sizeof(txt->sp_parm[i].nof_data);
		for(k = 0; k < p->nof_data; k++) {
			ltoh(p->parm[k], (char *)buf);
			buf += sizeof(txt->sp_parm[i].parm[k]);
		}
	}
	return(buf);
}
