/*======================================================================
 * File name    : rzi006.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	単重テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi006.cv  2.1  93/11/02 19:09:56  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi006.cv
 * Revision 2.1  93/11/02 19:09:56  ryoji
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
#include	<rzcmd\rzi005.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_piece_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	単重自動更新結果の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_piece_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_piece_weight(
	struct fmt_piece_weight	*txt,
	struct rzi005t		*pkt
	)
{
	BYTE		*buf;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->weight;
	for(i = 0; i < pkt->nof_sect ; i++) {
		itoh((unsigned short)pkt->weight[i], (char *)buf);
		buf += sizeof(txt->weight[0]);
	}
	return(buf);
}
