/*======================================================================
 * File name    : rzi036.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ネットワーク参加状態テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi036.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi036.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/RCS/rzi036.c 1.2 1994/11/04 18:54:04 ohtani
 *Log: rzi036.c
 * リビジョン 1.2  1994/11/04  18:54:04  ohtani
 * マップ格納バッファをビット・マップ化する。
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show rzcmd		*/
#include	<rzcmd\rzi035.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_netmap							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ネットワーク参加状態構造体のデータをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_netmap)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_netmap(
	struct fmt_netmap	*txt,
	struct rzi035t		*pkt
	)
{
	unsigned char	*s,*t;
	int		i;

	txt->brd_id = hex_ascii(pkt->brd_id);
	ctoh(pkt->dev_no, (char *)txt->dev_no);
	s = (unsigned char *)&pkt->net_map;
	t = (unsigned char *)&txt->net_map;
	for ( i=0; i<32; i++ ) {
		ctoh(*s++, (char *)t);
		t += 2;
	}
	itoh((unsigned short)pkt->recon, (char *)txt->recon);
	return(++txt);
}
