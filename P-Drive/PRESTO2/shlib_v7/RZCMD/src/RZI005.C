/*======================================================================
 * File name    : rzi005.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	単重テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi005.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi005.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi005.cv  2.1  93/11/02 19:10:10  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi005.cv
 * Revision 2.1  93/11/02 19:10:10  ryoji
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
#include	<rzcmd\rzi005.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_piece_weight						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	単重自動更新結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_piece_weight(
	struct fmt_piece_weight	*buf
	)
{
	BYTE		*p;
	int		i;

	rzi005s.ch_id = ascii_hex(buf->ch_id);
	rzi005s.nof_sect = ascii_hex(buf->nof_sect);
	for(i = 0, p = (PTR)&buf->weight; i < rzi005s.nof_sect ;
	    i++, p += sizeof(buf->weight[0])) {
		rzi005s.weight[i] = (unsigned short)htoi((char *)p);
	}
	return(0);
}
