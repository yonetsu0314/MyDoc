/*======================================================================
 * File name    : nzi010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi010.c 1.1 1999/05/27 16:51:07 kawamura
 *Log: nzi010.c
 * リビジョン 1.1  1999/05/27  16:51:07  kawamura
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
#include	<nzcmd\nzi009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_info							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各種データ応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_info)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_info(
	struct fmt_info		*txt,
	struct nzi009t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	ctoh((unsigned char)pkt->info_no, (char *)&txt->info_no);
	ctoh((unsigned char)pkt->info_desc, (char *)&txt->info_desc);
	ctoh((unsigned char)pkt->nof_data, (char *)&txt->nof_data);
	buf = (PTR)txt->data[0];
	/*ここから可変長データ処理*/
	for(i = 0 ; i < pkt->nof_data; i++) {
		ltoh((unsigned long)pkt->data[i], (char *)buf);
		buf += sizeof(txt->data[0]);
	}
	return(buf);
}
