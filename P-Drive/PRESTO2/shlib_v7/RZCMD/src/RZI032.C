/*======================================================================
 * File name    : rzi032.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi032.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi032.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi032.cv  1.1  94/02/21 20:18:24  ryoji
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi032.cv
 * Revision 1.1  94/02/21 20:18:24  ryoji
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
#include	<rzcmd\rzi031.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_cf_rslt						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF制御結果のデータをテキストに変換する。			*/
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
rz_print_cf_rslt(
	struct fmt_cf_rslt	*txt,
	struct rzi031t		*pkt
	)
{
	BYTE		*buf;
	RZI0311T	*p1;
	RZI0310T	*p0;
	int		i, j;

	txt->seq_no    = hex_ascii(pkt->seq_no);
	txt->nof_cf    = hex_ascii(pkt->nof_cf);
	buf = (PTR)txt->cf;
	for(i=0, p1=(PTR)&pkt->cf; i < pkt->nof_cf ; i++, p1++) {
		*buf = hex_ascii(p1->nof_sig);
		buf += sizeof(txt->cf[i].nof_sig);
		for(j=0, p0=(PTR)&p1->at; j < p1->nof_sig; j++, p0++) {
			ctoh(p0->amp, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].amp);
			ctoh(p0->time, (char *)buf);
			buf += sizeof(txt->cf[i].at[j].time);
		}
		ctoh(p1->sig_stat, (char *)buf);
		buf += sizeof(txt->cf[i].sig_stat);
		itoh((unsigned short)p1->df_wt, (char *)buf);
		buf += sizeof(txt->cf[i].df_wt);
	}
	return(buf);
}
