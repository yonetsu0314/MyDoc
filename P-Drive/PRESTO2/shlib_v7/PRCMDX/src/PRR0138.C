/*======================================================================
 * File name	: prr0138.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/rcs/prr0138.c 1.1 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0138.c $
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2007, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0137.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ext_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	拡張パラメータの構造体データをテキストに変換する。		*/
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
pr_print_ext_parm(
	struct fmt_ext_parm	*txt,
	struct prr0137t		*pkt
	)
{
	BYTE		*buf;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->sub_cmd, (char *)txt->sub_cmd);
	ctoh((unsigned char)pkt->nof_parm, (char *)txt->nof_parm);
	buf = (PTR)txt->parm;
	for(i = 0; i < pkt->nof_parm ; i++) {
		ltoh(pkt->parm[i], (char *)buf);
		buf += sizeof(txt->parm[i]);
	}
	return(buf);
}
