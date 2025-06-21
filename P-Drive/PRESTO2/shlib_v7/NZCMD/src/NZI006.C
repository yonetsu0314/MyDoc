/*======================================================================
 * File name    : nzi006.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ロジック波形データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi006.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi006.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi006.c 1.1 1999/05/27 16:50:20 kawamura
 *Log: nzi006.c
 * リビジョン 1.1  1999/05/27  16:50:20  kawamura
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
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<nzcmd\nzi005.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_logic							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ロジック波形データ応答構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_logic)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_logic(
	struct fmt_logic	*txt,
	struct nzi005t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->unit_no = hex_ascii( pkt->unit_no );
	ctoh((unsigned char)pkt->nof_data, (char *)&txt->nof_data);
	buf = (PTR)txt->data[0];
	/*ここから可変長データ処理*/
	for(i = 0 ; i < pkt->nof_data; i++) {
		itoh((unsigned short)pkt->data[i], (char *)buf);
		buf += sizeof(txt->data[0]);
	}
	return(buf);
}
