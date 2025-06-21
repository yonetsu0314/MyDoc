/*======================================================================
 * File name    : rzi010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi010.cv  2.2  94/04/12 17:40:00  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi010.cv
 * Revision 2.2  94/04/12 17:40:00  kabumoto
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
#include	<rzcmd\rzi009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_prgno							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	プログラム番号構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_prgno)	･･･ 正常終了			*/
/*		  NULL			･･･ エラー			*/
/*----------------------------------------------------------------------*/

void *
rz_print_prgno(
	struct fmt_prgno	*txt,
	struct rzi009t		*pkt
	)
{
	/* BYTE		*buf; */

	txt->dev_id = hex_ascii(pkt->dev_id);
	ctoh(pkt->head_no, (char *)txt->head_no);
	strncpy((char *)&txt->rom_no, (char *)&pkt->rom_no,
		sizeof(txt->rom_no));
	ctoh(pkt->ver_prefix, (char *)txt->ver_prefix);
	ctoh(pkt->ver_suffix, (char *)txt->ver_suffix);
	return(++txt);
}
