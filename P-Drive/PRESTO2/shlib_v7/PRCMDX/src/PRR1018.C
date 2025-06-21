/*======================================================================
 * File name	: prr1018.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 親ヘッドの遅れ時間拡張タイミングテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr1018.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr1018.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib_v7/rzcmdx/src/RCS/rzr218.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr218.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr1017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_parent_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	各親ヘッドの遅れ時間の構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tmg_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_parent_tmg(
	struct fmt_parent_tmg	*txt,
	struct prr1017t		*pkt
	)
{
	BYTE	*buf;
	int	i, j;

	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for( i = 0; i < pkt->nof_sect ; i++ ) {
		*buf = hex_ascii(pkt->sect[i].nof_parent);
		buf += sizeof(txt->sect[i].nof_parent);
		for( j = 0 ; j < pkt->sect[i].nof_parent; j++ ) {
			ctoh(pkt->sect[i].pandc[j].parent_delay, (char *)buf);
			buf += sizeof(txt->sect[i].pandc[j].parent_delay);
		}
	}
	return(buf);
}
