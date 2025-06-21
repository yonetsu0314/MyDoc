/*======================================================================
 * File name    : rzi024.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	システム異常発生テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi024.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi024.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi024.cv  2.2  94/04/12 17:38:50  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi024.cv
 * Revision 2.2  94/04/12 17:38:50  kabumoto
 * ワーニング除去
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
#include	<rzcmd\rzi023.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_sys_fault						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	システムエラー構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prgbo)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
rz_print_sys_fault(
	struct fmt_sys_fault	*txt,
	struct rzi023t		*pkt
	)
{
	ctoh(pkt->len, (char *)txt->len);
	strncpy((char *)txt->mes_str, (char *)&pkt->mes_str, (size_t)pkt->len);
	return(&txt->mes_str[pkt->len]);
}
