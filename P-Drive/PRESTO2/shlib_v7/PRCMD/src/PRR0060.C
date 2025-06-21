/*======================================================================
 * File name	: prr0060.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0060.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0060.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr060.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr060.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0059.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_pandc_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	親ヘッド計量目標値の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_pandc_parm(
	struct fmt_pandc_parm	*txt,
	struct prr0059t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for( i = 0; i < pkt->nof_sect ; i++ ) {
		*buf = hex_ascii(pkt->sect[i].nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		for( j = 0 ; j < pkt->sect[i].nof_phead; j++ ) {
			ctoh(pkt->sect[i].phead[j].no, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].no);
			itoh(pkt->sect[i].phead[j].upper_wt, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].upper_wt);
			itoh(pkt->sect[i].phead[j].lower_wt, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].lower_wt);
			*buf = hex_ascii(pkt->sect[i].phead[j].rf_ctrl);
			buf += sizeof(txt->sect[i].phead[j].rf_ctrl);
			ctoh(pkt->sect[i].phead[j].ph_fd, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].ph_fd);
		}
		*buf = hex_ascii(pkt->sect[i].on_off);
		buf += sizeof(txt->sect[i].on_off);
		*buf = hex_ascii(pkt->sect[i].dump);
		buf += sizeof(txt->sect[i].dump);
		*buf = hex_ascii(pkt->sect[i].calc);
		buf += sizeof(txt->sect[i].calc);
		itoh(pkt->sect[i].az_intvl, (char *)buf);
		buf += sizeof(txt->sect[i].az_intvl);
		*buf = hex_ascii(pkt->sect[i].over);
		buf += sizeof(txt->sect[i].over);
		*buf = hex_ascii(pkt->sect[i].under);
		buf += sizeof(txt->sect[i].under);
	}
	return(buf);
}
