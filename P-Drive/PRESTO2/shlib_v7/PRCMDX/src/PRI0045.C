/*======================================================================
 * File name    : pri0045.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動用状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0045.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0045.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi045.c 1.2 1999/03/05 11:58:06
 *Log: rzi045.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri0045.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_mlt_cond						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動用状態データのテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_mlt_cond(
	struct fmt_mlt_cond	*buf
	)
{
	BYTE		*p;
	int		i;

	pri0045s.ch_id = ascii_hex(buf->ch_id);
	pri0045s.cond_inf = htoc((char *)buf->cond_inf);
	pri0045s.act_inf = htoc((char *)buf->act_inf);
	pri0045s.node_ptn = htol((char *)buf->node_ptn);
	pri0045s.error_no = htoi((char *)buf->error_no);
	pri0045s.ex_d.data_cnt = htoc((char *)buf->ex_d.data_cnt);

	p = (PTR)&buf->ex_d.dt.c_data[0];
	for(i = 0; i < sizeof(pri0045s.ex_d.dt.c_data); i++) {
		pri0045s.ex_d.dt.c_data[i] = htoc((char *)p);
		p += 2;
	}
	pri0045s.ds_d.disp_cnt = htoc((char *)p);
	p += 2;
	strncpy((void *)&pri0045s.ds_d.disp_str, (void *)p, sizeof(pri0045s.ds_d.disp_str));
	p += sizeof(pri0045s.ds_d.disp_str);
	pri0045s.pr_d.prn_cnt = htoc((char *)p);
	p += 2;
	strncpy((void *)&pri0045s.pr_d.prn_str, (void *)p, sizeof(pri0045s.pr_d.prn_str));

	return(0);
}
