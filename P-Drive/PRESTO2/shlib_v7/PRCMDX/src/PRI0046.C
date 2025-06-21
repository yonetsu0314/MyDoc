/*======================================================================
 * File name    : pri0046.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動用状態データテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/pri0046.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0046.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi046.c 1.3 1999/03/10 10:57:21
 *Log: rzi046.c
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
#include	<prcmd\pri0045.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_mlt_cond						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動用状態データ構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_head_weight)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_mlt_cond(
	struct fmt_mlt_cond	*txt,
	struct pri0045t		*pkt
	)
{
	BYTE		*buf;
	int		i,f;

	txt->ch_id = hex_ascii(pkt->ch_id);
	ctoh((unsigned char)pkt->cond_inf, (char *)txt->cond_inf);
	ctoh((unsigned char)pkt->act_inf, (char *)txt->act_inf);
	ltoh(pkt->node_ptn, (char *)txt->node_ptn);
	itoh((unsigned short)pkt->error_no, (char *)txt->error_no);
	ctoh(pkt->ex_d.data_cnt, (char *)txt->ex_d.data_cnt);
	buf = (PTR)&txt->ex_d.dt.c_data[0];
	for(i = 0; i < sizeof(pkt->ex_d.dt.c_data); i++) {
		ctoh((unsigned char)pkt->ex_d.dt.c_data[i], (char *)buf);
		buf += 2;
	}
	ctoh(pkt->ds_d.disp_cnt, (char *)buf);
	buf += 2;
	for(i = 0, f = 0; i < sizeof(pkt->ds_d.disp_str); i++) {
		if(pkt->ds_d.disp_str[i] == '\0') f = 1;
		if(f)	*buf++ = '0';
		else	*buf++ = pkt->ds_d.disp_str[i];
	}
	ctoh(pkt->pr_d.prn_cnt, (char *)buf);
	buf += 2;
	for(i = 0, f = 0; i < sizeof(pkt->pr_d.prn_str); i++) {
		if(pkt->pr_d.prn_str[i] == '\0') f = 1;
		if(f)	*buf++ = '0';
		else	*buf++ = pkt->pr_d.prn_str[i];
	}

	return(buf);
}
