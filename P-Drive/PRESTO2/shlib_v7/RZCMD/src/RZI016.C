/*======================================================================
 * File name    : rzi016.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	警報発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi016.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi016.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi016.cv  2.1  93/11/02 19:12:04  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi016.cv
 * Revision 2.1  93/11/02 19:12:04  ryoji
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
#include	<rzcmd\rzi015.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_alarm							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	アラーム構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_alarm)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
rz_print_alarm(
	struct fmt_alarm	*txt,
	struct rzi015t		*pkt
	)
{
	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	txt->alarm = hex_ascii(pkt->alarm);
	ltoh((unsigned long)pkt->wh_ptn, (char *)txt->wh_ptn);
	ltoh((unsigned long)pkt->bh_ptn, (char *)txt->bh_ptn);
	ctoh(pkt->afd, (char *)txt->afd);
	return(++txt);
}
