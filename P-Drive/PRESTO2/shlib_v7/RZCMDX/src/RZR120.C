/*======================================================================
 * File name    : rzr120.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	４－２０ｍＡ出力に対するレベル重量スパンテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr120.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr120.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr120.c 1.3 1994/09/12 14:21:11 hiroe
 *Log: rzr120.c
 * リビジョン 1.3  1994/09/12  14:21:11  hiroe
 * 上下限偏差を上下限重量にする。
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
#include	<rzcmd\rzr119.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_dfspan							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	4-20mA出力に対するレベル重量スパンデータをテキストに変換する。	*/
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
rz_print_dfspan(
	struct fmt_dfspan	*txt,
	struct rzr119t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->min_df_no = hex_ascii( pkt->min_df_no );
	txt->nof_df    = hex_ascii( pkt->nof_df );

	buf = (BYTE_PTR)txt->spanwt;

	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh(pkt->spanwt[j].targ, (char *)buf);
		buf += sizeof(txt->spanwt[0].targ);
		itoh(pkt->spanwt[j].upper, (char *)buf);
		buf += sizeof(txt->spanwt[0].upper);
		itoh(pkt->spanwt[j].lower, (char *)buf);
		buf += sizeof(txt->spanwt[0].lower);
	}
	return(buf);
}
