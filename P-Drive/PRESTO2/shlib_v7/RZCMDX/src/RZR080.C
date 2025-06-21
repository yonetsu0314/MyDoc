/*======================================================================
 * File name    : rzr080.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト２作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr080.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr080.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr080.c 1.2 1996/04/11 17:17:17 hiroki
 *Log: rzr080.c
 * リビジョン 1.2  1996/04/11  17:17:17  hiroki
 * nof_cfを2BYTEに変更。
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
#include	<rzcmd\rzr079.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_cf2_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF振幅設定構造体のデータ２をテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_cf2_parm(
	struct fmt_cf2_parm	*txt,
	struct rzr079t		*pkt
	)
{
	BYTE		*buf;
	RZR0791T	*p1;
	RZR0790T	*p0;
	int		i, j;

	txt->ctrl      = hex_ascii(pkt->ctrl);
	ctoh(pkt->nof_cf, (char *)&txt->nof_cf);
	buf = (BYTE_PTR)txt->cf;
	for(i=0, p1=(PTR)&pkt->cf; i < pkt->nof_cf ; i++, p1++) {
		*buf = hex_ascii(p1->nof_sig);
		buf += sizeof(txt->cf[i].nof_sig);
		for(j=0, p0=(PTR)&p1->at; j < p1->nof_sig; j++, p0++) {
			ctoh(pkt->cf[i].at[j].amp, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].amp);
			ctoh(pkt->cf[i].at[j].time, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].time);
		}
	}
	return(buf);
}
