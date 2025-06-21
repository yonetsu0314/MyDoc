/*======================================================================
 * File name    : rzr023.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr023.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzr023.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:40
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:50
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr023.cv  2.1  93/11/02 19:19:08  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr023.cv
 * Revision 2.1  93/11/02 19:19:08  ryoji
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
#include	<rzcmd\rzr023.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_wei_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	計量目標値テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_wei_parm(
	struct fmt_wei_parm	*buf
	)
{
	P7		*p;
	BYTE		*p1;
	int		i;

	rzr023s.ch_id = ascii_hex(buf->ch_id);
	rzr023s.nof_head = htoc((char *)buf->nof_head);
	rzr023s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->sect; i < rzr023s.nof_sect ; i++, p++) {
		rzr023s.sect[i].assign_ptn = htol((char *)&p->assign_ptn);
		rzr023s.sect[i].target_wt = htol((char *)&p->target_wt);
		rzr023s.sect[i].upper_wt = (unsigned short)htoi((char *)&p->upper_wt);
		rzr023s.sect[i].tne = (unsigned short)htoi((char *)&p->tne);
		rzr023s.sect[i].target_cnt = (unsigned short)htoi((char *)&p->target_cnt);
		rzr023s.sect[i].upper_cnt = htoc((char *)&p->upper_cnt);
		rzr023s.sect[i].lower_cnt = htoc((char *)&p->lower_cnt);
		rzr023s.sect[i].piece_wt = (unsigned short)htoi((char *)&p->piece_wt);
		rzr023s.sect[i].target_head = htoc((char *)&p->target_head);
		rzr023s.sect[i].pw_ctrl = ascii_hex(p->pw_ctrl);
		rzr023s.sect[i].mix_ctrl = ascii_hex(p->mix_ctrl);
	}
	rzr023s.target_wt = htol((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->target_wt);
	p = (P7 *)p1;
	rzr023s.upper_wt = (unsigned short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->upper_wt);
	p = (P7 *)p1;
	rzr023s.tne = (unsigned short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->tne);
	p = (P7 *)p1;
	rzr023s.target_cnt = (unsigned short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->target_cnt);
	p = (P7 *)p1;
	rzr023s.upper_cnt = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->upper_cnt);
	p = (P7 *)p1;
	rzr023s.lower_cnt = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->lower_cnt);
	p = (P7 *)p1;
	rzr023s.speed = (unsigned short)htoi((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->speed);
	p = (P7 *)p1;
	rzr023s.dump_cnt = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->dump_cnt);
	p = (P7 *)p1;
	rzr023s.avg_ctrl = ascii_hex(*(BYTE_PTR)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->avg_ctrl);
	p = (P7 *)p1;
	rzr023s.cyc_of_under = htoc((char *)p);
	p1 = (BYTE_PTR)p;
	p1 += sizeof(buf->cyc_of_under);
	p = (P7 *)p1;
	return(0);
}
