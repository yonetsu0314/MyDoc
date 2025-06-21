/*======================================================================
 * File name    : rzr320.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	セクション毎のAFD設定テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr320.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr320.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr320.c 1.3 1997/11/04 17:07:54 hiroe
 *Log: rzr320.c
 * リビジョン 1.3  1997/11/04  17:07:54  hiroe
 * プログラムミス修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr319.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_sect_afd_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	セクション毎のAFD設定構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_sect_afd_parm(
	struct fmt_sect_afd_parm	*txt,
	struct rzr319t			*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->ch_id     = hex_ascii( pkt->ch_id );
	txt->nof_sect  = hex_ascii( pkt->nof_sect );
	buf = (BYTE_PTR)txt->afd_mode[0];
	for(i = 0; i < pkt->nof_sect; i++){
		*buf = hex_ascii( pkt->afd_mode[i] );
		buf += sizeof(txt->afd_mode[0]);
	}
	return(buf);
}
