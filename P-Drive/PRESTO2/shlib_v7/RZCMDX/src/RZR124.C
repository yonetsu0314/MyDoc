/*======================================================================
 * File name    : rzr124.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（１）テキスト作成
 *	優先参加回数
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr124.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr124.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr124.cv  1.2  94/05/27 13:46:42  fukutome
 *Log: p:/lib/rzcmdx/src/rcs/rzr124.cv
 * Revision 1.2  94/05/27 13:46:42  fukutome
 * 関数名の誤りを訂正
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
#include	<rzcmd\rzr123.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_prec_dump						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	優先参加回数構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prec_dump)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_prec_dump(
	struct fmt_prec_dump	*txt,
	struct rzr123t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);

	buf = (BYTE_PTR)txt->cnt[0];
	for(i = pkt->nof_sect, j = 0 ; i ; i--, j++) {
		ctoh(pkt->cnt[j], (char *)buf);
		buf += sizeof(txt->cnt[0]);
	}
	return buf;
}
