/*======================================================================
 * File name	: nzr156.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ共振周期設定テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmdx/src/RCS/nzr156.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr156.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmdx/src/rcs/nzr156.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr156.c
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
#include	<nzcmd\nzr155.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	nz_print_af_natural_fre						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ共振周期設定データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_af_drv_cycle)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
nz_print_af_natural_freq(
	struct fmt_af_natural_freq	*txt,
	struct nzr155t			*pkt
	)
{
	BYTE	*buf;
	int	i;

	buf = (BYTE_PTR)txt->nof_af;
	ctoh( (unsigned char)pkt->nof_af, (char *)buf );
	buf += sizeof(txt->nof_af);
	for( i = 0; i < pkt->nof_af; i++ ){
		itoh( (unsigned short)pkt->af_natural_freq[i], (char *)buf );
		buf += sizeof( txt->af_natural_freq[i] );
	}
	return(buf);

}
