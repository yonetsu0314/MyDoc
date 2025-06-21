/*======================================================================
 * File name	: prr0017.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0017.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0017.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr017.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr017.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_tmg_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	タイミングテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_tmg_parm(
	struct fmt_tmg_parm	*buf
	)
{
	BYTE	*p;
	PR00172	*p1;
	int	i,j;

	prr0017s.ch_id = ascii_hex( buf->ch_id );
	prr0017s.nof_head = htoc( (char *)buf->nof_head );

	prr0017s.ds_delay = htoc( (char *)buf->ds_delay );
	prr0017s.ds_hold_tm = htoc( (char *)buf->ds_hold_tm );
	prr0017s.op1_delay = htoc( (char *)buf->op1_delay );
	prr0017s.op1_hold_tm = htoc( (char *)buf->op1_hold_tm );
	prr0017s.op2_delay = htoc( (char *)buf->op2_delay );
	prr0017s.op2_hold_tm = htoc( (char *)buf->op2_hold_tm );
	prr0017s.wh_delay = htoc( (char *)buf->wh_delay );

	prr0017s.nof_round = ascii_hex( buf->nof_round );
	p = (BYTE_PTR)buf->round_base[0];
	for( i = 0; i < (int)prr0017s.nof_round; i++ ) {
		prr0017s.round_base[i] = htoc( (char *)p );
		p += sizeof(buf->round_base[i]);
	}

	prr0017s.min_th_no = htoc( (char *)p );
	p += sizeof(buf->min_th_no);
	prr0017s.nof_th = htoc( (char *)p );
	p += sizeof(buf->nof_th);
	for(i = prr0017s.nof_th, j = prr0017s.min_th_no-1 ; i ; i--, j++) {
		prr0017s.th[j].delay = htoc( (char *)p );
		p += sizeof(buf->th[j].delay);
		prr0017s.th[j].on = htoc( (char *)p );
		p += sizeof(buf->th[j].on);
	}

	prr0017s.min_dth_no = htoc( (char *)p );
	p += sizeof(buf->min_dth_no);
	prr0017s.nof_dth = htoc( (char *)p );
	p += sizeof(buf->nof_dth);
	for(i = prr0017s.nof_dth, j = prr0017s.min_dth_no-1 ; i ; i--, j++) {
		prr0017s.dth[j].delay = htoc( (char *)p );
		p += sizeof(buf->dth[j].delay);
		prr0017s.dth[j].on = htoc( (char *)p );
		p += sizeof(buf->dth[j].on);
	}

	prr0017s.min_rs_no = htoc( (char *)p );
	p += sizeof(buf->min_rs_no);
	prr0017s.nof_rs = htoc( (char *)p );
	p += sizeof(buf->nof_rs);
	for(i = prr0017s.nof_rs, j = prr0017s.min_rs_no-1 ; i ; i--, j++) {
		prr0017s.rs[j].delay = htoc( (char *)p );
		p += sizeof(buf->rs[j].delay);
		prr0017s.rs[j].pool = htoc( (char *)p );
		p += sizeof(buf->rs[j].pool);
	}

	prr0017s.nof_sect = ascii_hex( *p );
	p += sizeof(buf->nof_sect);
	p1 = (PR00172 *)p;
	for(i = 0 ; i < prr0017s.nof_sect ; i++, p1++) {
		prr0017s.sect[i].assign_ptn = htol( (char *)p1->assign_ptn );
		prr0017s.sect[i].wh_ph= htoc( (char *)p1->wh_ph );
		prr0017s.sect[i].ph_rf = htoc( (char *)p1->ph_rf );
		prr0017s.sect[i].wh1_bh = htoc( (char *)p1->wh1_bh );
		prr0017s.sect[i].bh_wh2 = htoc( (char *)p1->bh_wh2 );
		prr0017s.sect[i].stagger = htoc( (char *)p1->stagger );
		prr0017s.sect[i].sect_stagger = htoc( (char *)p1->sect_stagger );
		prr0017s.sect[i].bh_on = htoc( (char *)p1->bh_on );
		prr0017s.sect[i].wh_on = htoc( (char *)p1->wh_on );
		prr0017s.sect[i].ph_on = htoc( (char *)p1->ph_on );
	}
	return(0);
}
