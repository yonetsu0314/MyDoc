/*======================================================================
 * File name    : rzr017.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr017.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzr017.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:49
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr017.cv  2.2  93/11/04 11:05:06  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr017.cv
 * Revision 2.2  93/11/04 11:05:06  ryoji
 * バグフィックス
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
#define		STORAGE
#include	<rzcmd\rzr017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_tmg_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	タイミングテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_tmg_parm(
	struct fmt_tmg_parm	*buf
	)
{
	BYTE		*p1;
	P10		*p;
	int		i, j;

	rzr017s.ch_id    = ascii_hex( buf->ch_id );
	rzr017s.nof_head = htoc((char *) buf->nof_head );

	rzr017s.ds_delay   = htoc((char *) buf->ds_delay );
	rzr017s.ds_hold_tm = htoc((char *) buf->ds_hold_tm );
	rzr017s.err_delay  = htoc((char *) buf->err_delay );
	rzr017s.es_hold_tm = htoc((char *) buf->es_hold_tm );
	rzr017s.wh_delay   = htoc((char *) buf->wh_delay );

	rzr017s.nof_round    = ascii_hex( buf->nof_round );
	p1 = (PTR)&buf->round_base[0];
	for(i = rzr017s.nof_round, j = 0 ; i ; i--, j++) {
		rzr017s.round_base[j] = htoc((char *) p1 );
		p1 += sizeof(buf->round_base[0]);
	}

	rzr017s.min_th_no = ascii_hex( *p1 );
	p1 += sizeof(buf->min_th_no);
	rzr017s.nof_th    = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_th);
	for(i = rzr017s.nof_th, j = rzr017s.min_th_no-1 ; i ; i--, j++) {
		rzr017s.th[j].delay = htoc((char *) p1 );
		p1 += sizeof(buf->th[0].delay);
		rzr017s.th[j].on  = htoc((char *) p1 );
		p1 += sizeof(buf->th[0].on);
	}

	rzr017s.min_dth_no = ascii_hex( *p1 );
	p1 += sizeof(buf->min_dth_no);
	rzr017s.nof_dth    = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_dth);
	for(i = rzr017s.nof_dth, j = rzr017s.min_dth_no-1 ; i ; i--, j++) {
		rzr017s.dth[j].delay = htoc((char *) p1 );
		p1 += sizeof(buf->dth[0].delay);
		rzr017s.dth[j].on  = htoc((char *) p1 );
		p1 += sizeof(buf->dth[0].on);
	}

	rzr017s.min_rs_no = ascii_hex( *p1 );
	p1 += sizeof(buf->min_rs_no);
	rzr017s.nof_rs    = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_rs);
	for(i = rzr017s.nof_rs, j = rzr017s.min_rs_no-1 ; i ; i--, j++) {
		rzr017s.rs[j].delay = htoc((char *) p1 );
		p1 += sizeof(buf->rs[0].delay);
		rzr017s.rs[j].pool  = htoc((char *) p1 );
		p1 += sizeof(buf->rs[0].pool);
	}

	rzr017s.nof_sect = ascii_hex( *p1 );
	p1 += sizeof(buf->nof_sect);
	p = (P10 *)p1;
	for(i = 0 ; i < rzr017s.nof_sect ; i++, p++) {
		rzr017s.sect[i].assign_ptn  
				= (unsigned long)htol((char *) &p->assign_ptn );
		rzr017s.sect[i].wh_ph       = htoc((char *) &p->wh_ph );
		rzr017s.sect[i].ph_rf       = htoc((char *) &p->ph_rf );
		rzr017s.sect[i].wh1_bh      = htoc((char *) &p->wh1_bh );
		rzr017s.sect[i].bh_wh2      = htoc((char *) &p->bh_wh2 );
		rzr017s.sect[i].stagger     = htoc((char *) &p->stagger );
		rzr017s.sect[i].sect_stagger= htoc((char *) &p->sect_stagger );
		rzr017s.sect[i].bh_on       = htoc((char *) &p->bh_on );
		rzr017s.sect[i].wh_on       = htoc((char *) &p->wh_on );
		rzr017s.sect[i].ph_on       = htoc((char *) &p->ph_on );
	}
	return(0);
}
