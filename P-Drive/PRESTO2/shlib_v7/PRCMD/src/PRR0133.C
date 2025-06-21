/*======================================================================
 * File name	: prr0133.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	新ＡＦＤテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0133.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0133.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/05/28  14:04
 * 正しくデータが取得できない不具合を修正する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<stdio.h>
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0133.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_adv_afd							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	新ＡＦＤテキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_adv_afd(
	struct fmt_adv_afd	*buf
	)
{
	BYTE	*p;
	int	i,j,sect;

	prr0133s.nof_ch = ascii_hex(buf->nof_ch);
	p = (BYTE_PTR)buf->ch;
	for(i = 0 ; i < prr0133s.nof_ch ; i++ ) {
		prr0133s.ch[i].nof_sect = ascii_hex( *p );
		p += sizeof(buf->ch[i].nof_sect);
		for(sect = 0 ; sect < prr0133s.ch[i].nof_sect ; sect++ ) {
			for(j = 0 ; j < sizeof(prr0133s.ch[i].sect[sect].tbl) ; j++ ) {
				prr0133s.ch[i].sect[sect].tbl[j] = htoc( (char *)p );
				p += sizeof(buf->ch[i].sect[sect].tbl[j]);
			}
			prr0133s.ch[i].sect[sect].zero_d_num = htoc( (char *)p );
			p += sizeof(buf->ch[i].sect[sect].zero_d_num);
			prr0133s.ch[i].sect[sect].p_os_c_num = htoc( (char *)p );
			p += sizeof(buf->ch[i].sect[sect].p_os_c_num);
			prr0133s.ch[i].sect[sect].trb_off_head = htoc( (char *)p );
			p += sizeof(buf->ch[i].sect[sect].trb_off_head);
			prr0133s.ch[i].sect[sect].cln_req_sw = htoc( (char *)p );
			p += sizeof(buf->ch[i].sect[sect].cln_req_sw);
			prr0133s.ch[i].sect[sect].good_effi = htoi( (char *)p );
			p += sizeof(buf->ch[i].sect[sect].good_effi);
		}
	}
	prr0133s.nof_infeed = ascii_hex( *p );
	p += sizeof(buf->nof_infeed);
	for(i = 0 ; i < prr0133s.nof_infeed ; i++ ) {
		prr0133s.ifd[i].df_fuzzy_gain = htoi( (char *)p );
		p += sizeof(buf->ifd[i].df_fuzzy_gain);
		prr0133s.ifd[i].df_l_detect = htoi( (char *)p );
		p += sizeof(buf->ifd[i].df_l_detect);
		prr0133s.ifd[i].df_ow_ctrl = htoc( (char *)p );
		p += sizeof(buf->ifd[i].df_ow_ctrl);
		prr0133s.ifd[i].dfw_on_off = htoc( (char *)p );
		p += sizeof(buf->ifd[i].dfw_on_off);
		prr0133s.ifd[i].df_e_detect1 = htoc( (char *)p );
		p += sizeof(buf->ifd[i].df_e_detect1);
		prr0133s.ifd[i].df_e_detect2 = htoc( (char *)p );
		p += sizeof(buf->ifd[i].df_e_detect2);
		prr0133s.ifd[i].df_w_max = htoi( (char *)p );
		p += sizeof(buf->ifd[i].df_w_max);
		prr0133s.ifd[i].df_w_min = htoi( (char *)p );
		p += sizeof(buf->ifd[i].df_w_min);
		prr0133s.ifd[i].low_wt = htoc( (char *)p );
		p += sizeof(buf->ifd[i].low_wt);
	}
	return(0);
}
