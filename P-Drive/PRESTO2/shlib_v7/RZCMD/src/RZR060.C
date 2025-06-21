/*======================================================================
 * File name    : rzr060.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr060.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr060.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr060.cv  1.1  94/02/18 13:34:20  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr060.cv
 * Revision 1.1  94/02/18 13:34:20  ryoji
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
#include	<rzcmd\rzr059.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_pandc_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量目標値の構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_pandc_parm(
	struct fmt_pandc_parm	*txt,
	struct rzr059t		*pkt
	)
{
	BYTE		*buf;
	RZR0591T	*p1;
	RZR0590T	*p0;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i=0, p1=(PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii(p1->nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		for(j=0, p0=(PTR)&p1->phead; j < p1->nof_phead; j++, p0++) {
			ctoh(p0->no, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].no);
			itoh((unsigned short)p0->upper_wt, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].upper_wt);
			itoh((unsigned short)p0->lower_wt, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].lower_wt);
			*buf = hex_ascii(p0->rf_ctrl);
			buf += sizeof(txt->sect[i].phead[j].rf_ctrl);
			ctoh(p0->ph_fd, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].ph_fd);
		}
		*buf = hex_ascii(p1->on_off);
		buf += sizeof(txt->sect[i].on_off);
		*buf = hex_ascii(p1->dump);
		buf += sizeof(txt->sect[i].dump);
		*buf = hex_ascii(p1->calc);
		buf += sizeof(txt->sect[i].calc);
		itoh((unsigned short)p1->az_intvl, (char *)buf);
		buf += sizeof(txt->sect[i].az_intvl);
		*buf = hex_ascii(p1->over);
		buf += sizeof(txt->sect[i].over);
		*buf = hex_ascii(p1->under);
		buf += sizeof(txt->sect[i].under);
	}
	return(buf);
}
