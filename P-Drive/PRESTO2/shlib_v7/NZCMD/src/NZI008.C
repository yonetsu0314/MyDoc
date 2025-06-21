/*======================================================================
 * File name    : nzi008.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi008.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi008.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi008.c 1.1 1999/05/27 16:50:13 kawamura
 *Log: nzi008.c
 * リビジョン 1.1  1999/05/27  16:50:13  kawamura
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
#include	<nzcmd\nzi007.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_archive						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	重量波形データ応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_archive)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_archive(
	struct fmt_archive	*txt,
	struct nzi007t		*pkt
	)
{
	NI0070	*buf;
	int	i;

	ctoh((unsigned char)pkt->wh_no, (char *)&txt->wh_no);
	itoh((unsigned short)pkt->index, (char *)&txt->index);
	ctoh((unsigned char)pkt->nof_data, (char *)&txt->nof_data);
	buf = txt->data;
	/*ここから可変長データ処理*/
	for(i = 0 ; i < pkt->nof_data; i++) {
		itoh((unsigned short)pkt->data[i].weight, (char *)buf->weight);
		buf->status = hex_ascii( pkt->data[i].status );
		++buf;
	}
	return(buf);
}
