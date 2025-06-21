/*======================================================================
 * File name	: pri0047.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ係数テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0047.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0047.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/dzcmd/src/RCS/dzi203.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: dzi203.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri0047.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_afv_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＦＶ係数テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_afv_coeffi(
	struct fmt_afv_coeffi	*buf
	)
{
	pri0047s.wh_no = htoc( (char *)buf->wh_no );
	pri0047s.new_coeffi = (long)htol((char *)buf->new_coeffi);
	pri0047s.old_coeffi = (long)htol((char *)buf->old_coeffi);
	return(0);
}
