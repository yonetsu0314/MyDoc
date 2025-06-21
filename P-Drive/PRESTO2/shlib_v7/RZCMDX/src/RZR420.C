/*======================================================================
 * File name	: rzr420.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ供給量テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr420.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr420.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr420.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr420.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr419.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_af_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ供給量設定構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_af_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_af_parm(
	struct fmt_af_parm	*txt,
	struct rzr419t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	ctoh( pkt->min_af_no , (char *)txt->min_af_no );
	ctoh( pkt->nof_af , (char *)txt->nof_af );

	buf = (BYTE_PTR)txt->af;
	for(i = pkt->nof_af, j = pkt->min_af_no-1 ; i ; i--, j++) {
		ctoh(pkt->af[j].amp, (char *)buf);
		buf += sizeof(txt->af[0].amp);
		ctoh(pkt->af[j].time, (char *)buf);
		buf += sizeof(txt->af[0].time);
	}
	return(buf);
}
