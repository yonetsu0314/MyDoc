/*======================================================================
 * File name    : rzr024.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr024.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr024.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr024.cv  2.1  93/11/02 19:19:26  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr024.cv
 * Revision 2.1  93/11/02 19:19:26  ryoji
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
#include	<rzcmd\rzr023.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_wei_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量目標値の構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_wei_parm(
	struct fmt_wei_parm	*txt,
	struct rzr023t		*pkt
	)
{
	BYTE		*buf;
	RZR0230T	*p;
	int		i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh(pkt->nof_head, (char *)&txt->nof_head);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (PTR)txt->sect;
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		ltoh((unsigned long)p->assign_ptn, (char *)buf);
		buf += sizeof(txt->sect[i].assign_ptn);
		ltoh((unsigned long)p->target_wt, (char *)buf);
		buf += sizeof(txt->sect[i].target_wt);
		itoh((unsigned short)p->upper_wt, (char *)buf);
		buf += sizeof(txt->sect[i].upper_wt);
		itoh((unsigned short)p->tne, (char *)buf);
		buf += sizeof(txt->sect[i].tne);
		itoh((unsigned short)p->target_cnt, (char *)buf);
		buf += sizeof(txt->sect[i].target_cnt);
		ctoh(p->upper_cnt, (char *)buf);
		buf += sizeof(txt->sect[i].upper_cnt);
		ctoh(p->lower_cnt, (char *)buf);
		buf += sizeof(txt->sect[i].lower_cnt);
		itoh((unsigned short)p->piece_wt, (char *)buf);
		buf += sizeof(txt->sect[i].piece_wt);
		ctoh(p->target_head, (char *)buf);
		buf += sizeof(txt->sect[i].target_head);
		*buf = hex_ascii(p->pw_ctrl);
		buf += sizeof(txt->sect[i].pw_ctrl);
		*buf = hex_ascii(p->mix_ctrl);
		buf += sizeof(txt->sect[i].mix_ctrl);
	}
	ltoh((unsigned long)pkt->target_wt, (char *)buf);
	buf += sizeof(txt->target_wt);
	itoh((unsigned short)pkt->upper_wt, (char *)buf);
	buf += sizeof(txt->upper_wt);
	itoh((unsigned short)pkt->tne, (char *)buf);
	buf += sizeof(txt->tne);
	itoh((unsigned short)pkt->target_cnt, (char *)buf);
	buf += sizeof(txt->target_cnt);
	ctoh(pkt->upper_cnt, (char *)buf);
	buf += sizeof(txt->upper_cnt);
	ctoh(pkt->lower_cnt, (char *)buf);
	buf += sizeof(txt->lower_cnt);
	itoh((unsigned short)pkt->speed, (char *)buf);
	buf += sizeof(txt->speed);
	ctoh(pkt->dump_cnt, (char *)buf);
	buf += sizeof(txt->dump_cnt);
	*buf = hex_ascii(pkt->avg_ctrl);
	buf += sizeof(txt->avg_ctrl);
	ctoh(pkt->cyc_of_under, (char *)buf);
	buf += sizeof(txt->cyc_of_under);
	return(buf);
}
