/*======================================================================
 * File name	: prr0059.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0059.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0059.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr059.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr059.c
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
#include	<prcmd\prr0059.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_pandc_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	親ヘッド計量目標値テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_pandc_parm(
	struct fmt_pandc_parm	*buf
	)
{
	BYTE	*p;
	int	i,j;

	prr0059s.ch_id = ascii_hex(buf->ch_id);
	prr0059s.nof_head = htoc((char *)buf->nof_head);
	prr0059s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ; i < prr0059s.nof_sect ; i++ ) {
		prr0059s.sect[i].nof_phead = ascii_hex(*p);
		p += sizeof(buf->sect[i].nof_phead);
		for( j = 0 ; j < prr0059s.sect[i].nof_phead ; j++ ) {
			prr0059s.sect[i].phead[j].no = htoc((char *)p);
			p += sizeof(buf->sect[i].phead[j].no);
			prr0059s.sect[i].phead[j].upper_wt = htoi((char *)p);
			p += sizeof(buf->sect[i].phead[j].upper_wt);
			prr0059s.sect[i].phead[j].lower_wt = htoi((char *)p);
			p += sizeof(buf->sect[i].phead[j].lower_wt);
			prr0059s.sect[i].phead[j].rf_ctrl = ascii_hex(*p);
			p += sizeof(buf->sect[i].phead[j].rf_ctrl);
			prr0059s.sect[i].phead[j].ph_fd = htoc((char *)p);
			p += sizeof(buf->sect[i].phead[j].ph_fd);
		}
		prr0059s.sect[i].on_off = ascii_hex(*p);
		p += sizeof(buf->sect[i].on_off);
		prr0059s.sect[i].dump = ascii_hex(*p);
		p += sizeof(buf->sect[i].dump);
		prr0059s.sect[i].calc = ascii_hex(*p);
		p += sizeof(buf->sect[i].calc);
		prr0059s.sect[i].az_intvl = htoi((char *)p);
		p += sizeof(buf->sect[i].az_intvl);
		prr0059s.sect[i].over = ascii_hex(*p);
		p += sizeof(buf->sect[i].over);
		prr0059s.sect[i].under = ascii_hex(*p);
		p += sizeof(buf->sect[i].under);
	}
	return(0);
}
