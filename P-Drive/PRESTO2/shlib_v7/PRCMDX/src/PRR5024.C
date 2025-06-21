/*======================================================================
 * File name    : prr5024.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（５）テキスト作成
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr5024.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr5024.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr524.c 1.1 1970/01/01
 *Log: rzr524.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr5023.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_ext_tgt						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張上下限値データの構造体をテキストに変換する			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_sample_size)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ext_tgt(
	struct fmt_ext_tgt 	*txt,
	struct prr5023t		*pkt
	)
{
	BYTE		*buf;
	PRR50230T	*p;
	int		i;

	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		ltoh((unsigned long)p->upper_wt, (char *)buf);
		buf += sizeof(txt->sect[i].upper_wt);
		ltoh((unsigned long)p->lower_wt, (char *)buf);
		buf += sizeof(txt->sect[i].lower_wt);
		itoh(p->upper_cnt, (char *)buf);
		buf += sizeof(txt->sect[i].upper_cnt);
		itoh(p->lower_cnt, (char *)buf);
		buf += sizeof(txt->sect[i].lower_cnt);
	}
	return(buf);
}
