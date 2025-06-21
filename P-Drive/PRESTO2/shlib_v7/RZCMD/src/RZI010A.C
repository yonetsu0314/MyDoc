/*======================================================================
 * File name    : rzi010a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用プログラム番号テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi010a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi010a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi010a.c 1.1 1995/11/21 09:21:04 hiroe
 *Log: rzi010a.c
 * リビジョン 1.1  1995/11/21  09:21:04  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzi009a.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_prgno_v3						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	Ver3用プログラム番号構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prgno_v3)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_prgno_v3(
	struct fmt_prgno_v3	*txt,
	struct rzi009t		*pkt
	)
{
	/* BYTE		*buf; */

	txt->brd_id = hex_ascii(pkt->brd_id);
	txt->dev_no = hex_ascii(pkt->dev_no);
	ctoh(pkt->node_id, (char *)txt->node_id);
	ctoh(pkt->len, (char *)txt->len);
	strncpy((char *)&txt->rom_id, (char *)&pkt->rom_id, (size_t)pkt->len);
	return &txt->rom_id[pkt->len];
}
