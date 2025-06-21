/*======================================================================
 * File name    : rzr218.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 拡張タイミングテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr218.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr218.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr218.c 1.2 1996/08/05 10:26:02 hiroe
 *Log: rzr218.c
 * リビジョン 1.2  1996/08/05  10:26:02  hiroe
 * i -> j
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
#include	<rzcmd\rzr217.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_parent_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各親ヘッドの遅れ時間の構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tmg_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_parent_tmg(
	struct fmt_parent_tmg	*txt,
	struct rzr217t		*pkt
	)
{
	BYTE		*buf;
	RZR2170T	*p1;
	RZR2171T	*p2;
	int		i, j;

	buf = (BYTE_PTR)&txt->nof_sect;
	*buf = hex_ascii( pkt->nof_sect );
	buf += sizeof(txt->nof_sect);
	for(i = 0, p1 = (PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii( p1->nof_parent );
		buf += sizeof(txt->sect[i].nof_parent);
		for(j=0, p2=(PTR)&p1->pandc; j < p1->nof_parent; j++, p2++) {
			ctoh(p2->parent_delay, (char *)buf);
			buf += sizeof(txt->sect[i].pandc[j].parent_delay);
		}
	}
	return(buf);
}
