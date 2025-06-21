/*======================================================================
 * File name	: nzi152.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ共振周期検出開始テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmdx/src/RCS/nzr152.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr152.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmdx/src/rcs/nzr152.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr152.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#include	<nzcmd\nzr151.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	nz_print_af_res_detect						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ共振周期検出開始データをテキストに変換する		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_af_resonance_detect)･･･ 正常終		*/
/*----------------------------------------------------------------------*/

void *
nz_print_af_res_detect(
	struct fmt_af_resonance_detect	*txt,
	struct nzr151t			*pkt
	)
{
	BYTE	*buf;

	buf = (BYTE_PTR)txt->af_detect_ptn;
	ltoh ( (unsigned long)pkt->af_detect_ptn, (char *)buf );
	buf += sizeof(txt->af_detect_ptn);
	return(buf);

}
