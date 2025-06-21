/*======================================================================
 * File name	: pri0030.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0030.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0030.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi030.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi030.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/**/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri0029.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	親ヘッド計量結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	 							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_pandc_result(
	struct fmt_pandc_result	*txt,
	struct pri0029t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;
	char	str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for( i = 0 ; i < pkt->nof_sect ; i++ ) {
		*buf = hex_ascii( pkt->sect[i].nof_phead );
		buf += sizeof(txt->sect[i].nof_phead);
		for( j = 0 ; j < pkt->sect[i].nof_phead; j++ ) {
			itoh((unsigned short)pkt->sect[i].phead[j].weight, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].weight);
			itoh((unsigned short)pkt->sect[i].phead[j].wh_status, (char *)str);
			memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->sect[i].phead[j].wh_status));
			buf += sizeof(txt->sect[i].phead[j].wh_status);
			itoh((unsigned short)pkt->sect[i].phead[j].bh_status, (char *)str);
			memcpy((PTR)buf, (PTR)&str[1], sizeof(txt->sect[i].phead[j].bh_status));
			buf += sizeof(txt->sect[i].phead[j].bh_status);
		}
	}
	return(buf);
}
