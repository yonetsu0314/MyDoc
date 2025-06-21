/*======================================================================
 * File name    : rzr056.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤヘッドデータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr056.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr056.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr056.cv  1.1  93/11/02 19:26:42  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr056.cv
 * Revision 1.1  93/11/02 19:26:42  ryoji
 * 第５版対応
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
#include	<rzcmd\rzr055.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_afd_head						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＦＤヘッドデータ構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_afd_head)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_afd_head(
	struct fmt_afd_head	*txt,
	struct rzr055t		*pkt
	)
{
	P92		*buf;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->min_wh_no, (char *)txt->min_wh_no);
	ctoh(pkt->nof_wh, (char *)txt->nof_wh);
	buf = txt->wh;
	for(i = pkt->nof_wh, j = pkt->min_wh_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->wh[j].trg, (char *)buf->trg);
		itoh((unsigned short)pkt->wh[j].std, (char *)buf->std);
		buf++;
	}
	return(buf);
}
