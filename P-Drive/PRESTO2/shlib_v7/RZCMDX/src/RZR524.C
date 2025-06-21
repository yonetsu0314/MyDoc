/*======================================================================
 * File name    : rzr524.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（５）テキスト作成
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr524.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr524.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr524.c 1.2 1999/07/07 17:58:11 itami
 *Log: rzr524.c
 * リビジョン 1.2  1999/07/07  17:58:11  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr523.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_ext_tgt						*/
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
rz_print_ext_tgt(
	struct fmt_ext_tgt 	*txt,
	struct rzr523t		*pkt
	)
{
	BYTE		*buf;
	RZR5230T	*p;
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
