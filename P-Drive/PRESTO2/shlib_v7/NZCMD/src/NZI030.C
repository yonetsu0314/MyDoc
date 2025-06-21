/*======================================================================
 * File name    : nzi030.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi030.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi030.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi030.c 1.1 1999/05/27 16:50:40 kawamura
 *Log: nzi030.c
 * リビジョン 1.1  1999/05/27  16:50:40  kawamura
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
#include	<custom\xmemory.h>		/**/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<nzcmd\nzi029.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	nz_print_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量結果構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_pandc_result(
	struct fmt_pandc_result_nz	*txt,
	struct nzi029t			*pkt
	)
{
	BYTE		*buf;
	NZI0290T	*p0;
	NZI0291T	*p1;
	int		i, j;
	char		str[4];

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->nof_head, (char *)&txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p1 = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii(p1->nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		for(j=0, p0=(PTR)&p1->phead; j < p1->nof_phead; j++, p0++) {
			itoh((unsigned short)p0->weight, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].weight);
			itoh((unsigned short)p0->wh_status, (char *)str);
			memcpy((void *)buf, (void *)&str[1], sizeof(txt->sect[i].phead[j].wh_status));
			buf += sizeof(txt->sect[i].phead[j].wh_status);
			itoh((unsigned short)p0->bh_status, (char *)str);
			memcpy((void *)buf, (void *)&str[1], sizeof(txt->sect[i].phead[j].bh_status));
			buf += sizeof(txt->sect[i].phead[j].bh_status);
		}
	}
	return(buf);
}
