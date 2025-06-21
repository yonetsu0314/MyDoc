/*======================================================================
 * File name    : rzi026.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	平均値制御結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi026.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi026.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi026.cv  1.2  93/11/27 08:51:32  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi026.cv
 * Revision 1.2  93/11/27 08:51:32  ryoji
 * 個数を0.1個単位にする
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
#include	<rzcmd\rzi025.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_mean_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	平均値制御結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_mean_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_mean_result(
	struct fmt_mean_result	*txt,
	struct rzi025t		*pkt
	)
{
	BYTE		*buf;
	RZI0250T	*p;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		ltoh((unsigned long)p->weight, (char *)buf);
		buf += sizeof(txt->sect[i].weight);
		ltoh((unsigned long)p->count, (char *)buf);
		buf += sizeof(txt->sect[i].count);
	}
	ltoh((unsigned long)pkt->ch.weight, (char *)buf);
	buf += sizeof(txt->ch.weight);
	ltoh((unsigned long)pkt->ch.count, (char *)buf);
	buf += sizeof(txt->ch.count);
	return(buf);
}
