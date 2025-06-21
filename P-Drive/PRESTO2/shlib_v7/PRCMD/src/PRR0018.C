/*======================================================================
 * File name	: prr0018.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	 タイミングテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0018.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr018.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr018.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_tmg_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	タイミング構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tmg_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_tmg_parm(
	struct fmt_tmg_parm	*txt,
	struct prr0017t		*pkt
	)
{
	BYTE		*buf;
	PRR00171T	*p;
	int		i, j;

	txt->ch_id = hex_ascii( pkt->ch_id );
	ctoh( pkt->nof_head, (char *)txt->nof_head );
	ctoh( pkt->ds_delay, (char *)txt->ds_delay );
	ctoh( pkt->ds_hold_tm, (char *)txt->ds_hold_tm );
	ctoh( pkt->op1_delay, (char *)txt->op1_delay );
	ctoh( pkt->op1_hold_tm, (char *)txt->op1_hold_tm );
	ctoh( pkt->op2_delay, (char *)txt->op2_delay );
	ctoh( pkt->op2_hold_tm, (char *)txt->op2_hold_tm );
	ctoh( pkt->wh_delay, (char *)txt->wh_delay );
	txt->nof_round = hex_ascii( pkt->nof_round );
	buf = (PTR)txt->round_base[0];
	for( i = 0; i < (int)pkt->nof_round; i++ ) {
		ctoh( pkt->round_base[i], (char *)buf );
		buf += sizeof(txt->round_base[i]);
	}
	ctoh( pkt->min_th_no, (char *)buf );
	buf += sizeof(txt->min_th_no);
	ctoh( pkt->nof_th, (char *)buf );
	buf += sizeof(txt->nof_th);
	for(i = pkt->nof_th, j = pkt->min_th_no-1 ; i ; i--, j++) {
		ctoh(pkt->th[j].delay, (char *)buf);
		buf += sizeof(txt->th[j].delay);
		ctoh(pkt->th[j].on, (char *)buf);
		buf += sizeof(txt->th[j].on);
	}
	ctoh( pkt->min_dth_no, (char *)buf );
	buf += sizeof(txt->min_dth_no);
	ctoh( pkt->nof_dth, (char *)buf );
	buf += sizeof(txt->nof_dth);
	for(i = pkt->nof_dth, j = pkt->min_dth_no-1 ; i ; i--, j++) {
		ctoh(pkt->dth[j].delay, (char *)buf);
		buf += sizeof(txt->dth[j].delay);
		ctoh(pkt->dth[j].on, (char *)buf);
		buf += sizeof(txt->dth[j].on);
	}
	ctoh( pkt->min_rs_no, (char *)buf );
	buf += sizeof(txt->min_rs_no);
	ctoh( pkt->nof_rs, (char *)buf );
	buf += sizeof(txt->nof_rs);
	for(i = pkt->nof_rs, j = pkt->min_rs_no-1 ; i ; i--, j++) {
		ctoh(pkt->rs[j].delay, (char *)buf);
		buf += sizeof(txt->rs[j].delay);
		ctoh(pkt->rs[j].pool, (char *)buf);
		buf += sizeof(txt->rs[j].pool);
	}
	*buf = hex_ascii( pkt->nof_sect );
	buf += sizeof(txt->nof_sect);
	for(i = 0, p = (PTR)pkt->sect; i < pkt->nof_sect ; i++, p++) {
		ltoh( p->assign_ptn, (char *)buf);
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
