/*======================================================================
 * File name	: rzr204.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ構成仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr204.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr204.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr204.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr204.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr203.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	rz_print_af_construct						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	補助フィーダ構成仕様データをテキストに変換する			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信テキスト・バッファへのポインタ    		*/
/*	*pkt	: パラメータ・パケットへのポインタ   			*/
/*									*/
/* 戻り値								*/
/*	int	: buf+sizeof(fmt_af_construct)	･･･ 正常終了		*/
/*----------------------------------------------------------------------*/

void *
rz_print_af_construct(
	struct fmt_af_construct	*txt,
	struct rzr203t		*pkt
	)
{
	BYTE		*buf = (BYTE_PTR)txt;
	int		i;

	ctoh( pkt->nof_af, (char *)txt->nof_af );
	buf = (BYTE_PTR)txt->af_assign_ptn[0];
	for( i = 0; i < pkt->nof_af; i++ ){
		ltoh( pkt->af_assign_ptn[i], (char *)buf );
		buf += sizeof( txt->af_assign_ptn[i] );
	}
	return(buf);
}
