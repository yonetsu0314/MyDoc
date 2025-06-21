/*======================================================================
 * File name    : rzi045.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動用状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi045.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi045.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi045.c 1.2 1999/03/05 11:58:06 itami
 *Log: rzi045.c
 * リビジョン 1.2  1999/03/05  11:58:06  itami
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
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzi045.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_mlt_cond						*/
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
rz_scan_mlt_cond(
	struct fmt_mlt_cond	*buf
	)
{
	BYTE	*p;
	int	i;

	rzi045s.ch_id = ascii_hex(buf->ch_id);
	rzi045s.cond_inf = htoc((char *)buf->cond_inf);
	rzi045s.act_inf = htoc((char *)buf->act_inf);
	rzi045s.node_ptn = htol((char *)buf->node_ptn);
	rzi045s.error_no = htoi((char *)buf->error_no);
	rzi045s.ex_d.data_cnt = htoc((char *)buf->ex_d.data_cnt);

	p = (BYTE_PTR)&buf->ex_d.dt.c_data[0];
	for(i = 0; i < sizeof(rzi045s.ex_d.dt.c_data); i++) {
		rzi045s.ex_d.dt.c_data[i] = htoc((char *)p);
		p += 2;
	}
	rzi045s.ds_d.disp_cnt = htoc((char *)p);
	p += 2;
	strncpy((char *)&rzi045s.ds_d.disp_str, (char *)p, sizeof(rzi045s.ds_d.disp_str));
	p += sizeof(rzi045s.ds_d.disp_str);
	rzi045s.pr_d.prn_cnt = htoc((char *)p);
	p += 2;
	strncpy((char *)&rzi045s.pr_d.prn_str, (char *)p, sizeof(rzi045s.pr_d.prn_str));

	return(0);
}
