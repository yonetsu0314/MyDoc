/*======================================================================
 * File name    : rzr318.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 タイミングテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr318.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr318.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr318.c 1.1 1996/11/22 19:10:45 hiroe
 *Log: rzr318.c
 * リビジョン 1.1  1996/11/22  19:10:45  hiroe
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
#include	<rzcmd\rzr317.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_round_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ラウンディングデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_round_tmg)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_round_tmg(
	struct fmt_round_tmg	*txt,
	struct rzr317t		*pkt
	)
{
	BYTE		*buf;
	int		i, j;

	txt->nof_round  = hex_ascii( pkt->nof_round );
	buf = (BYTE_PTR)txt->round_base[0];
	for(i = pkt->nof_round, j = 0 ; i ; i--, j++) {
		ctoh( pkt->round_base[j], (char *)buf );
		buf += sizeof(txt->round_base[0]);
	}
	return(buf);
}
