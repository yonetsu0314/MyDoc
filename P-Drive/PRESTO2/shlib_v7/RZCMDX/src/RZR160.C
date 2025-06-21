/*======================================================================
 * File name    : rzr160.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ＢＨオーバースケール重量テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr160.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr160.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr160.c 1.2 1996/05/09 19:20:00 hiroe
 *Log: rzr160.c
 * リビジョン 1.2  1996/05/09  19:20:00  hiroe
 * bh_reject_weight の型を変更する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr159.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_p_reject_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ＢＨオーバースケール重量の構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_p_reject_parm(
	struct fmt_p_reject_parm	*txt,
	struct rzr159t		*pkt
	)
{
	BYTE		*buf;
	RZR1591T	*p1;
	RZR1590T	*p0;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i=0, p1=(PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii(p1->nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		for(j=0, p0=(PTR)&p1->phead; j < p1->nof_phead; j++, p0++) {
			ltoh((unsigned long)p0->bh_reject_wt, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].bh_reject_wt);
		}
	}
	return(buf);
}
