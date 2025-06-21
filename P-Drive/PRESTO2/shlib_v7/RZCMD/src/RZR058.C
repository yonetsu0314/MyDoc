/*======================================================================
 * File name    : rzr058.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr058.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr058.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr058.cv  1.1  93/11/02 19:37:30  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr058.cv
 * Revision 1.1  93/11/02 19:37:30  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr057.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_afd_sect						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＦＤセクションの構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_afd_sect)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_afd_sect(
	struct fmt_afd_sect	*txt,
	struct rzr057t		*pkt
	)
{
	BYTE		*buf;
	RZR0570T	*p;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		*buf = hex_ascii(p->ctrl_mode);
		buf += sizeof(txt->sect[i].ctrl_mode);
		itoh((unsigned short)p->ctrl_sw, (char *)buf);
		buf += sizeof(txt->sect[i].ctrl_sw);
		itoh((unsigned short)p->start_delay, (char *)buf);
		buf += sizeof(txt->sect[i].start_delay);
		ctoh(p->df_rf_ratio, (char *)buf);
		buf += sizeof(txt->sect[i].df_rf_ratio);
		ctoh(p->target_head, (char *)buf);
		buf += sizeof(txt->sect[i].target_head);
		ctoh(p->amp_center, (char *)buf);
		buf += sizeof(txt->sect[i].amp_center);
		ctoh(p->amp_max, (char *)buf);
		buf += sizeof(txt->sect[i].amp_max);
		ctoh(p->amp_min, (char *)buf);
		buf += sizeof(txt->sect[i].amp_min);
		ctoh(p->amp_range, (char *)buf);
		buf += sizeof(txt->sect[i].amp_range);
	}
	return(buf);
}
