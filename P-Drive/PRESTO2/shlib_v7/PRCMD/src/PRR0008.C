/*======================================================================
 * File name	: prr0008.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	包装機仕様テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0008.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0008.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr008.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr008.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0007.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_bmifspec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	包装機連動仕様構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_bmif_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_bmif_spec(
	struct fmt_bmif_spec	*txt,
	struct prr0007t		*pkt
	)
{
	BYTE		*buf;
	PRR00070T	*p;
	int		i;

	ctoh( pkt->nof_head, (char *)&txt->nof_head );
	txt->nof_bm = hex_ascii(pkt->nof_bm);
	buf = (BYTE_PTR)txt->ch;
	for(i = 0, p = (PTR)pkt->ch; i < (int)pkt->nof_bm ; i++, p++) {
		ltoh( p->bm_assign_ptn, (char *)buf );
		buf += sizeof(txt->ch[i].bm_assign_ptn);
		*buf = hex_ascii( p->interface );
		buf += sizeof(txt->ch[i].interface);
		*buf = hex_ascii( p->mul_dump_ini );
		buf += sizeof(txt->ch[i].mul_dump_ini);
		*buf = hex_ascii(p->mul_dump_conf );
		buf += sizeof(txt->ch[i].mul_dump_conf);
		*buf = hex_ascii(p->manu_dump_ini );
		buf += sizeof(txt->ch[i].manu_dump_ini);
		*buf = hex_ascii(p->manu_dump_conf );
		buf += sizeof(txt->ch[i].manu_dump_conf);
	}
	return(buf);
}
