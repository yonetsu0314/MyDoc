/*======================================================================
 * File name    : rzr018.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 タイミングテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr018.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr018.cv  2.1  93/11/02 19:18:32  ryoji  Exp
 *Log: p:/lib/rzcmd/src/rcs/rzr018.cv
 * Revision 2.1  93/11/02 19:18:32  ryoji
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
#include	<rzcmd\rzr017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_tmg_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	タイミング構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tmg_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_tmg_parm(
	struct fmt_tmg_parm	*txt,
	struct rzr017t		*pkt
	)
{
	BYTE		*buf;
	RZR0171T	*p;
	int		i, j;

	txt->ch_id = hex_ascii( pkt->ch_id );
	ctoh( pkt->nof_head, (char *)&txt->nof_head );
	ctoh( pkt->ds_delay, (char *)&txt->ds_delay );
	ctoh( pkt->ds_hold_tm, (char *)&txt->ds_hold_tm );
	ctoh( pkt->err_delay, (char *)&txt->err_delay );
	ctoh( pkt->es_hold_tm, (char *)&txt->es_hold_tm );
	ctoh( pkt->wh_delay, (char *)&txt->wh_delay );

	txt->nof_round  = hex_ascii( pkt->nof_round );
	buf = (PTR)txt->round_base[0];
	for(i = pkt->nof_round, j = 0 ; i ; i--, j++) {
		ctoh( pkt->round_base[j], (char *)buf );
		buf += sizeof(txt->round_base[0]);
	}

	*buf = hex_ascii( pkt->min_th_no );
	buf += sizeof(txt->min_th_no);
	*buf   = hex_ascii( pkt->nof_th );
	buf += sizeof(txt->nof_th);
	for(i = pkt->nof_th, j = pkt->min_th_no-1 ; i ; i--, j++) {
		ctoh(pkt->th[j].delay, (char *)buf);
		buf += sizeof(txt->th[0].delay);
		ctoh(pkt->th[j].on, (char *)buf);
		buf += sizeof(txt->th[0].on);
	}

	*buf = hex_ascii( pkt->min_dth_no );
	buf += sizeof(txt->min_dth_no);
	*buf   = hex_ascii( pkt->nof_dth );
	buf += sizeof(txt->nof_dth);
	for(i = pkt->nof_dth, j = pkt->min_dth_no-1 ; i ; i--, j++) {
		ctoh(pkt->dth[j].delay, (char *)buf);
		buf += sizeof(txt->dth[0].delay);
		ctoh(pkt->dth[j].on, (char *)buf);
		buf += sizeof(txt->dth[0].on);
	}

	*buf = hex_ascii( pkt->min_rs_no );
	buf += sizeof(txt->min_rs_no);
	*buf   = hex_ascii( pkt->nof_rs );
	buf += sizeof(txt->nof_rs);
	for(i = pkt->nof_rs, j = pkt->min_rs_no-1 ; i ; i--, j++) {
		ctoh(pkt->rs[j].delay, (char *)buf);
		buf += sizeof(txt->rs[0].delay);
		ctoh(pkt->rs[j].pool, (char *)buf);
		buf += sizeof(txt->rs[0].pool);
	}

	*buf = hex_ascii( pkt->nof_sect );
	buf += sizeof(txt->nof_sect);
	for(i = 0, p = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p++) {
		ltoh((unsigned long) p->assign_ptn, (char *)buf);
		buf += sizeof(txt->sect[i].assign_ptn);
		ctoh(p->wh_ph, (char *)buf);
		buf += sizeof(txt->sect[i].wh_ph);
		ctoh(p->ph_rf, (char *)buf);
		buf += sizeof(txt->sect[i].ph_rf);
		ctoh(p->wh1_bh, (char *)buf);
		buf += sizeof(txt->sect[i].wh1_bh);
		ctoh(p->bh_wh2, (char *)buf);
		buf += sizeof(txt->sect[i].bh_wh2);
		ctoh(p->stagger, (char *)buf);
		buf += sizeof(txt->sect[i].stagger);
		ctoh(p->sect_stagger, (char *)buf);
		buf += sizeof(txt->sect[i].sect_stagger);
		ctoh(p->bh_on, (char *)buf);
		buf += sizeof(txt->sect[i].bh_on);
		ctoh(p->wh_on, (char *)buf);
		buf += sizeof(txt->sect[i].wh_on);
		ctoh(p->ph_on, (char *)buf);
		buf += sizeof(txt->sect[i].ph_on);
	}
	return(buf);
}
