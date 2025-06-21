/*======================================================================
 * File name    : rzr152.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張テスト駆動開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr152.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr152.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr152.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzr152.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
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
#include	<rzcmd\rzr151.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ext_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張テスト駆動停止構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ext_tstdrv_run)･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_ext_tstdrv_run(
	struct fmt_ext_tstdrv_run	*txt,
	struct rzr151t			*pkt
	)
{
	ltoh(pkt->sht_ptn, (char *)&txt->sht_ptn);
	txt->sht_flg = hex_ascii(pkt->sht_flg);
	ltoh(pkt->spa_ptn, (char *)&txt->spa_ptn);
	txt->spa_flg = hex_ascii(pkt->spa_flg);
	return(++txt);
}
