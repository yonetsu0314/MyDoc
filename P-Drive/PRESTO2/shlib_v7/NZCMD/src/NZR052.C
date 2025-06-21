/*======================================================================
 * File name	: nzi052.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr052.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr052.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr052.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr052.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<nzcmd\nzr051.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	nz_print_res_detect						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ共振周期検出開始データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_resonance_detect)･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
nz_print_res_detect(
	struct fmt_resonance_detect	*txt,
	struct nzr051t			*pkt
	)
{
	BYTE	*buf;

	buf = (BYTE_PTR)txt->rf_detect_ptn;
	ltoh ( (unsigned long)pkt->rf_detect_ptn, (char *)buf );
	buf += sizeof(txt->rf_detect_ptn);
	ltoh ( (unsigned long)pkt->df_detect_ptn, (char *)buf );
	buf += sizeof(txt->df_detect_ptn);
	ltoh ( (unsigned long)pkt->cf_detect_ptn, (char *)buf );
	buf += sizeof(txt->cf_detect_ptn);
	return(buf);

}
