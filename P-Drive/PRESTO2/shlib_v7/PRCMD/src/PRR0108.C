/*======================================================================
 * File name	: prr0108.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0108.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0108.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzr052.c 1.1 1970/01/01 00:00:00 chimura Exp
 *$log: nzr052.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0107.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_res_detect						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ共振周期検出開始データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信テキスト・バッファへのポインタ			*/
/*	*pkt	: パラメータ・パケットへのポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_resonance_detect)･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
pr_print_res_detect(
	struct fmt_resonance_detect	*txt,
	struct prr0107t			*pkt
	)
{
	BYTE	*buf;

	buf = (BYTE_PTR)txt->rf_detect_ptn;
	ltoh ( pkt->rf_detect_ptn, (char *)buf );
	buf += sizeof(txt->rf_detect_ptn);
	ltoh ( pkt->df_detect_ptn, (char *)buf );
	buf += sizeof(txt->df_detect_ptn);
	ltoh ( pkt->af_detect_ptn, (char *)buf );
	buf += sizeof(txt->af_detect_ptn);
	ltoh ( pkt->cf_detect_ptn, (char *)buf );
	buf += sizeof(txt->cf_detect_ptn);
	return(buf);

}
