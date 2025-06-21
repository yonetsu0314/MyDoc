/*======================================================================
 * File name    : rzr076.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡパラメータコマンド　　　テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr076.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr076.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr076.c 1.4 1996/07/12 19:27:54 hiroki
 *Log: rzr076.c
 * リビジョン 1.4  1996/07/12  19:27:54  hiroki
 * フリトパラを、ＤＣＡパラに変更する。
 * ＡＴＡ９６仕様
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
#include	<rzcmd\rzr075.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_dca_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＤＣＡパラメータコマンドをテキストに変換する。	    		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *rz_print_dca_parm(
	struct fmt_dca_parm	*txt,
	struct rzr075t		*pkt
	)
{
	txt->dca_on_off = hex_ascii( pkt->dca_on_off );
	itoh( (unsigned short)pkt->dca_cycle, (char *)&txt->dca_cycle );
	ctoh( pkt->dca_owf, (char *)&txt->dca_owf );
	ctoh( pkt->dca_sdm, (char *)&txt->dca_sdm );
	return(++txt);
}
