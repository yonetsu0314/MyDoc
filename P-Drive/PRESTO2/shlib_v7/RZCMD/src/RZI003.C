/*======================================================================
 * File name    : rzi003.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi003.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzi003.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:38
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:44
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi003.cv  2.1  93/11/02 19:08:10  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi003.cv
 * Revision 2.1  93/11/02 19:08:10  ryoji
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
#define		STORAGE
#include	<rzcmd\rzi003.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cb_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	組み合わせ計算結果テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cb_result(
	struct fmt_cb_result	*buf
	)
{
	P8		*p;
	BYTE		*p1;
	int		i, j;

	rzi003s.ch_id = ascii_hex(buf->ch_id);
	rzi003s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->sect; i < rzi003s.nof_sect ;
	    i++, p++) {
		rzi003s.sect[i].flag = ascii_hex(p->flag);
		rzi003s.sect[i].weight = htol((char *)&p->weight);
		rzi003s.sect[i].count = (short)htoi((char *)&p->count);
	}
	rzi003s.ch.flag = ascii_hex(p->flag);
	rzi003s.ch.weight = htol((char *)&p->weight);
	rzi003s.ch.count = (short)htoi((char *)&p->count);
	p++;
	rzi003s.slv_targ_wt = htol((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->slv_targ_wt);
	p = (P8 *)p1;
	rzi003s.slv_targ_cnt = (short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->slv_targ_cnt);
	p = (P8 *)p1;
	rzi003s.min_wh_no = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->min_wh_no);
	p = (P8 *)p1;
	rzi003s.nof_wh = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_wh);
	p = (P8 *)p1;
	for(i = rzi003s.nof_wh, j = rzi003s.min_wh_no-1 ; i ; i--, j++) {
		rzi003s.wh_status[j] = htoc((char *)p);
		p1 = (BYTE_PTR)p;
		p1 += sizeof(buf->wh_status[0]);
		p = (P8 *)p1;
	}
	rzi003s.nof_bh = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->nof_bh);
	p = (P8 *)p1;
	for(i = rzi003s.nof_bh, j = rzi003s.min_wh_no-1 ; i ; i--, j++) {
		rzi003s.bh_status[j] = htoc((char *)p);
		p1 = (BYTE_PTR)p;
		p1 += sizeof(buf->bh_status[0]);
		p = (P8 *)p1;
	}
	return(0);
}
