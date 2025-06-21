/*======================================================================
 * File name    : nzr010.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	安定判定時間 拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr010.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr010.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr010.c 1.1 1999/06/17 10:11:32 kawamura
 *Log: nzr010.c
 * リビジョン 1.1  1999/06/17  10:11:32  kawamura
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
#include	<nzcmd\nzr009.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	nz_print_wei_spec1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	安定判定時間 拡張パラメータ構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	void *	: txt+sizeof(fmt_wei_spec1_nz)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
nz_print_wei_spec1(
	struct fmt_wei_spec1_nz	*txt,
	struct nzr009t		*pkt
	)
{
	BYTE		*buf;
	NZR0091T	*p1;
	int		i,j;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	buf = (PTR)txt->ch;
	for(i=0, p1=(PTR)&pkt->ch; i < pkt->nof_ch ; i++, p1++) {
		*buf = hex_ascii(p1->nof_sect);
		buf += sizeof(txt->ch[i].nof_sect);
		for(j=0; j < p1->nof_sect; j++) {
			itoh((unsigned short)pkt->ch[i].sect[j].stable_time, (char *)buf);
			buf += sizeof(txt->ch[i].sect[j].stable_time);
		}
	}
	return(buf);
}
