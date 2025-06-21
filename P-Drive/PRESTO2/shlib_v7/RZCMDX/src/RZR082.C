/*======================================================================
 * File name    : rzr082.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイマーコントロールテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr082.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr082.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr082.c 1.5 1996/09/19 15:39:05 hiroki
 *Log: rzr082.c
 * リビジョン 1.5  1996/09/19  15:39:05  hiroki
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
#include	<rzcmd\rzr081.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_timer_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	タイマ設定内容構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_timer_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_timer_parm(
	struct fmt_timer_parm	*txt,
	struct rzr081t		*pkt
	)
{
	BYTE		*buf;
	RZR0811T	*p1;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_port = hex_ascii( pkt->nof_port );

	buf = (BYTE_PTR)txt->timer;

	for(i = 0, p1=(PTR)&pkt->timer; i < pkt->nof_port; i++ ,p1++) {
		*buf = hex_ascii(p1->port);
		buf += sizeof(txt->timer[i].port);
		ctoh(p1->init, (char *)buf);
		buf += sizeof(txt->timer[i].init);
		ctoh(p1->off, (char *)buf);
		buf += sizeof(txt->timer[i].off);
		ctoh(p1->on, (char *)buf);
		buf += sizeof(txt->timer[i].on);
	}
	return(buf);
}
