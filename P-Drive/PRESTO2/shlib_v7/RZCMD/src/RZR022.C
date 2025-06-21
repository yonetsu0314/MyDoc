/*======================================================================
 * File name    : rzr022.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr022.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr022.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr022.cv  2.2  94/02/21 20:10:36  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr022.cv
 * Revision 2.2  94/02/21 20:10:36  ryoji
 * ダブル機シングルＣＦ対応
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
#include	<rzcmd\rzr021.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_cf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF振幅設定構造体のデータをテキストに変換する。			*/
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
rz_print_cf_parm(
	struct fmt_cf_parm	*txt,
	struct rzr021t		*pkt
	)
{
	BYTE		*buf;
	RZR0211T	*p1;
	RZR0210T	*p0;
	int		i, j;

	txt->ctrl      = hex_ascii(pkt->ctrl);
	txt->nof_cf    = hex_ascii(pkt->nof_cf);
	buf = (PTR)txt->cf;
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
