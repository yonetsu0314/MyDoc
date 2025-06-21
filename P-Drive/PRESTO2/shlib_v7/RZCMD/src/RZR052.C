/*======================================================================
 * File name    : rzr052.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr052.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr052.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr052.cv  2.1  93/11/02 19:23:56  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr052.cv
 * Revision 2.1  93/11/02 19:23:56  ryoji
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
#include	<rzcmd\rzr051.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	テスト駆動停止構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tstdrv_run)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_tstdrv_run(
	struct fmt_tstdrv_run	*txt,
	struct rzr051t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	ctoh(pkt->df_ptn, (char *)&txt->df_ptn);
	ltoh((unsigned long)pkt->rf_ptn, (char *)&txt->rf_ptn);
	ltoh((unsigned long)pkt->ph_ptn, (char *)&txt->ph_ptn);
	ltoh((unsigned long)pkt->wh_ptn, (char *)&txt->wh_ptn);
	ltoh((unsigned long)pkt->wh2_ptn, (char *)&txt->wh2_ptn);
	ltoh((unsigned long)pkt->bh_ptn, (char *)&txt->bh_ptn);
	ctoh(pkt->rs_ptn, (char *)&txt->rs_ptn);
	ctoh(pkt->dth_ptn, (char *)&txt->dth_ptn);
	ctoh(pkt->th_ptn, (char *)&txt->th_ptn);
	txt->drv_cyc = hex_ascii( pkt->drv_cyc );
	return(++txt);
}
