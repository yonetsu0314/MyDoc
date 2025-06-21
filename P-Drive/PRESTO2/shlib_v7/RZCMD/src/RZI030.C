/*======================================================================
 * File name    : rzi030.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi030.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi030.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi030.cv  1.1  94/02/18 13:34:30  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi030.cv
 * Revision 1.1  94/02/18 13:34:30  ryoji
 * Initial revision
 * 
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
#include	<rzcmd\rzi029.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_pandc_result(
	struct fmt_pandc_result	*txt,
	struct rzi029t		*pkt
	)
{
	BYTE		*buf;
	RZI0290T	*p0;
	RZI0291T	*p1;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p1 = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii(p1->nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		for(j=0, p0=(PTR)&p1->phead; j < p1->nof_phead; j++, p0++) {
			itoh((unsigned short)p0->weight, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].weight);
			ctoh((unsigned char)p0->wh_status, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].wh_status);
			ctoh((unsigned char)p0->bh_status, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].bh_status);
		}
	}
	return(buf);
}
