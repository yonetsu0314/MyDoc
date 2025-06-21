/*======================================================================
 * File name    : rzr007.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	包装機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr007.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr007.cv  2.1  93/11/02 19:16:56  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr007.cv
 * Revision 2.1  93/11/02 19:16:56  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr007.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_bmif_spec						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	包装機連動仕様テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_bmif_spec(
	struct fmt_bmif_spec	*buf
	)
{
	P2		*p;
	int		i;

	rzr007s.nof_head = htoc((char *) buf->nof_head );
	rzr007s.nof_bm  = ascii_hex( buf->nof_bm );
	p = (PTR)&buf->ch;
	for(i = 0 ; i < rzr007s.nof_bm ; i++, p++) {
		rzr007s.ch[i].bm_assign_ptn = htol((char *) &p->bm_assign_ptn );
		rzr007s.ch[i].interface = ascii_hex( p->interface );
		rzr007s.ch[i].mul_dump_ini = ascii_hex( p->mul_dump_ini );
		rzr007s.ch[i].mul_dump_conf = ascii_hex( p->mul_dump_conf );
		rzr007s.ch[i].manu_dump_ini = ascii_hex( p->manu_dump_ini );
		rzr007s.ch[i].manu_dump_conf = ascii_hex(p->manu_dump_conf );
	}
	return(0);
}
