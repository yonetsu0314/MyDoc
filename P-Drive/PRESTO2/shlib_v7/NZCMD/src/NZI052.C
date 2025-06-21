/*======================================================================
 * File name	: nzi052.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期応答テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzi052.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi052.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzi052.c 1.2 1970/01/01 00:00:00 kawamura
 *Log: nzi052.c
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2001/07/25 15:44
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
#include	<nzcmd\nzi051.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	nz_print_resonance						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ共振周期応答データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_resonance)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
nz_print_resonance(
	struct fmt_resonance	*txt,
	struct nzi051t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	buf = (BYTE_PTR)txt->nof_rf;
	ctoh( (unsigned char)pkt->nof_rf, (char *)buf );
	buf += sizeof(txt->nof_rf);
	for( i = 0; i < pkt->nof_rf; i++ ){
		itoh( (unsigned short)pkt->rf_res_cycle[i], (char *)buf );
		buf += sizeof( txt->rf_res_cycle[i] );
	}
	ctoh( (unsigned char)pkt->nof_df, (char *)buf );
	buf += sizeof( txt->nof_df );
	for( i = 0; i < pkt->nof_df; i++ ){
		itoh( (unsigned short)pkt->df_res_cycle[i], (char *)buf );
		buf += sizeof( txt->df_res_cycle[i] );
	}
	ctoh( (unsigned char)pkt->nof_cf, (char *)buf );
	buf += sizeof( txt->nof_cf );
	for( i = 0; i < pkt->nof_cf; i++ ){
		itoh( (unsigned short)pkt->cf_res_cycle[i], (char *)buf );
		buf += sizeof( txt->cf_res_cycle[i] );
	}
	return(buf);

}
