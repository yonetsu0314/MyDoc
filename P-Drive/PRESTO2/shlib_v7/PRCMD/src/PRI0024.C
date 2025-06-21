/*======================================================================
 * File name	: pri0024.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	システム異常発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0024.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0024.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi024.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi024.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0023.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_sys_fault						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	システムエラー構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prgbo)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
pr_print_sys_fault(
	struct fmt_sys_fault	*txt,
	struct pri0023t		*pkt
	)
{
	ctoh(pkt->len, (char *)txt->len);
	strncpy((char *)txt->mes_str, (char *)&pkt->mes_str, (size_t)pkt->len);
	return(&txt->mes_str[pkt->len]);
}
