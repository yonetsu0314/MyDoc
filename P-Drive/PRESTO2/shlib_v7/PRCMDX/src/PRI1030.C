/*======================================================================
 * File name	: pri1030.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 補助組み合わせ候補重量データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri1030.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri1030.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\pri1029.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ref_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助組み合わせ候補重量データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ref_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ref_weight(
	struct fmt_ref_weight	*txt,
	struct pri1029t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for( i = 0; i < pkt->nof_sect ; i++ ) {
		*buf = hex_ascii(pkt->sect[i].nof_data);
		buf += sizeof(txt->sect[i].nof_data);
		for( j = 0 ; j < pkt->sect[i].nof_data; j++ ) {
			itoh((unsigned short)pkt->sect[i].weight[j], (char *)buf);
			buf += sizeof(txt->sect[i].weight[j]);
		}
	}
	return(buf);
}
