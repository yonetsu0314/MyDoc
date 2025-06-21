/*======================================================================
 * File name    : rzr064.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	上下動シュートパラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr064.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr064.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr064.c 1.2 1995/05/02 14:15:33 hiroe
 *Log: rzr064.c
 * リビジョン 1.2  1995/05/02  14:15:33  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom/typedefs.h>		/* Show xlib		*/
#include	<custom/custom.h>		/*	"		*/
#include	<rzcmd/rzcmd.h>			/* Show Current		*/
#include	<rzcmd/rzr063.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_prec_dump						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	上下動シュートパラメータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(updown_shoot_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_updown_shoot(
	struct updown_shoot_parm	*txt,
	struct rzr063t	*pkt
	)
{
	BYTE	*buf;

	buf = (BYTE_PTR)txt->dev_id;
	ctoh(pkt->dev_id, (char *)buf);
	buf += sizeof(txt->dev_id);
	ctoh(pkt->nof_pos, (char *)buf);
	buf += sizeof(txt->nof_pos);
	ctoh(pkt->err_detect_interval, (char *)buf);
	buf += sizeof(txt->err_detect_interval);
	ctoh(pkt->position, (char *)buf);
	buf += sizeof(txt->position);
	return buf;
}
