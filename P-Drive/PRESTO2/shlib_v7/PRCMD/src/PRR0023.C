/*======================================================================
 * File name	: prr0023.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0023.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0023.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr023.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr023.c
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
#include	<prcmd\prr0023.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_wei_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	計量目標値テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_wei_parm(
	struct fmt_wei_parm	*buf
	)
{
	PR00230	*p1;
	BYTE	*p;
	int	i;

	prr0023s.ch_id = ascii_hex(buf->ch_id);
	prr0023s.nof_head = htoc((char *)buf->nof_head);
	prr0023s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p1 = (PTR)buf->sect; i < prr0023s.nof_sect ; i++, p1++) {
		prr0023s.sect[i].assign_ptn = htol((char *)p1->assign_ptn);
		prr0023s.sect[i].target_wt = (long)htol((char *)p1->target_wt);
		prr0023s.sect[i].upper_wt = htoi((char *)p1->upper_wt);
		prr0023s.sect[i].upper_wt2 = htoi((char *)p1->upper_wt2);
		prr0023s.sect[i].tne = htoi((char *)p1->tne);
		prr0023s.sect[i].target_cnt = (short)htoi((char *)p1->target_cnt);
		prr0023s.sect[i].upper_cnt = htoc((char *)p1->upper_cnt);
		prr0023s.sect[i].upper_cnt2 = htoc((char *)p1->upper_cnt2);
		prr0023s.sect[i].lower_cnt = htoc((char *)p1->lower_cnt);
		prr0023s.sect[i].piece_wt = htoi((char *)p1->piece_wt);
		prr0023s.sect[i].target_head = htoc((char *)p1->target_head);
		prr0023s.sect[i].pw_ctrl = ascii_hex(p1->pw_ctrl);
		prr0023s.sect[i].mix_ctrl = ascii_hex(p1->mix_ctrl);
		prr0023s.sect[i].cnt = htoc((char *)p1->cnt);
	}
	p = (BYTE_PTR)p1;
	prr0023s.target_wt = htol((char *)p);
	p += sizeof(buf->target_wt);
	prr0023s.upper_wt = htoi((char *)p);
	p += sizeof(buf->upper_wt);
	prr0023s.upper_wt2 = htoi((char *)p);
	p += sizeof(buf->upper_wt2);
	prr0023s.tne = htoi((char *)p);
	p += sizeof(buf->tne);
	prr0023s.target_cnt = (short)htoi((char *)p);
	p += sizeof(buf->target_cnt);
	prr0023s.upper_cnt = htoc((char *)p);
	p += sizeof(buf->upper_cnt);
	prr0023s.upper_cnt2 = htoc((char *)p);
	p += sizeof(buf->upper_cnt2);
	prr0023s.lower_cnt = htoc((char *)p);
	p += sizeof(buf->lower_cnt);
	prr0023s.speed = htoi((char *)p);
	p += sizeof(buf->speed);
	prr0023s.dump_cnt = htoc((char *)p);
	p += sizeof(buf->dump_cnt);
	prr0023s.avg_ctrl = ascii_hex(*(BYTE_PTR)p);
	p += sizeof(buf->avg_ctrl);
	prr0023s.second_upper_cycle = htoi((char *)p);
	p += sizeof(buf->second_upper_cycle);
	prr0023s.cyc_of_under = htoc((char *)p);
	p += sizeof(buf->cyc_of_under);
	return(0);
}
