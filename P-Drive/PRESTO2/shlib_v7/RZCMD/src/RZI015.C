/*======================================================================
 * File name    : rzi015.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	警報発生テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi015.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi015.cv  2.1  93/11/02 19:12:16  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzi015.cv
 * Revision 2.1  93/11/02 19:12:16  ryoji
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
#include	<rzcmd\rzi015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_alarm							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	アラームテキストを構造体にセーブする。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_alarm(
	struct fmt_alarm	*buf
	)
{
	rzi015s.ch_id = ascii_hex(buf->ch_id);
	rzi015s.nof_head = htoc((char *)buf->nof_head);
	rzi015s.alarm = ascii_hex(buf->alarm);
	rzi015s.wh_ptn = htol((char *)buf->wh_ptn);
	rzi015s.bh_ptn = htol((char *)buf->bh_ptn);
	rzi015s.afd = htoc((char *)buf->afd);
	return(0);
}
