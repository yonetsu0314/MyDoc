/*======================================================================
 * File name	: prr2010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 ＰＨセルデータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr2010.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr2010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr2009.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_ph_cell						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＰＨセルの構造体データをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_ph_cell)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_ph_cell(
	struct fmt_ph_cell	*txt,
	struct prr2009t		*pkt
	)
{
	BYTE	*buf;
	int	i,j;

	txt->ph_span_adj = hex_ascii( pkt->ph_span_adj );
	txt->nof_ch = hex_ascii( pkt->nof_ch );
	buf = (BYTE_PTR)txt->ch;
	for( i = 0; i < (int)pkt->nof_ch; i++ ) {
		itoh( pkt->ch[i].stable_time, (char *)buf );
		buf += sizeof( txt->ch[i].stable_time );
		ctoh( pkt->ch[i].zero_interval, (char *)buf );
		buf += sizeof( txt->ch[i].zero_interval );
		*buf = hex_ascii( pkt->ch[i].nof_sect );
		buf += sizeof( txt->ch[i].nof_sect );
		for( j = 0; j < (int)pkt->ch[i].nof_sect; j++ ) {
			itoh( pkt->ch[i].sect[j].empty_weight, (char *)buf );
			buf += sizeof( txt->ch[i].sect[j].empty_weight );
			itoh( pkt->ch[i].sect[j].stick_weight, (char *)buf );
			buf += sizeof( txt->ch[i].sect[j].stick_weight );
			*buf = hex_ascii( pkt->ch[i].sect[j].ph_weight_check );
			buf += sizeof(txt->ch[i].sect[j].ph_weight_check);
		}
	}
	return(buf);
}
