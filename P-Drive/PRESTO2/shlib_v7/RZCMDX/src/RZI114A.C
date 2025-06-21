/*======================================================================
 * File name    : rzi114a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	本体状態情報拡張パラメータテキスト作成（拡張１）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi114a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi114a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi114a.c 1.1 1996/07/26 11:22:25 hiroki
 *Log: rzi114a.c
 * リビジョン 1.1  1996/07/26  11:22:25  hiroki
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
#include	<rzcmd\rzi113a.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_ata_target						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ata_target)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_ata_target(
	struct fmt_ata_target		*txt,
	struct rzi113t			*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	/*ここから可変長データ処理*/
	buf = (BYTE_PTR)txt->ata_target[0];
	for(i = 0; i < pkt->nof_ch ; i++) {
		ltoh( pkt->ata_target[i], (char *)buf);
		buf += sizeof(txt->ata_target[i]);
	}
	return(buf);
}
