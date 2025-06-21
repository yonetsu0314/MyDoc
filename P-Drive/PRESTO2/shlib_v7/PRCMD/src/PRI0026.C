/*======================================================================
 * File name	: pri0026.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	各種データ応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0026.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0026.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi010.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi010.c
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
#include	<prcmd\pri0025.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_info							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	各種データ応答構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_info)		･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_info(
	struct fmt_info		*txt,
	struct pri0025t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	ctoh((unsigned char)pkt->info_no, (char *)txt->info_no);
	ctoh(pkt->info_desc, (char *)txt->info_desc);
	ctoh(pkt->nof_data, (char *)txt->nof_data);
	buf = (BYTE_PTR)txt->data[0];
	for(i = 0 ; i < pkt->nof_data; i++) {
		ltoh((unsigned long)pkt->data[i], (char *)buf);
		buf += sizeof(txt->data[i]);
	}
	return(buf);
}
