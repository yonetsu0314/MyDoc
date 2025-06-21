/*======================================================================
 * File name	: nzr051.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr051.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr051.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr051.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: nzr051.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
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
#define		STORAGE
#include	<nzcmd\nzr051.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_res_detect						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	フィーダ共振周期検出開始を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

int
nz_scan_res_detect( 
	struct fmt_resonance_detect	*buf
	)
{
	nzr051s.rf_detect_ptn = htol((char *)buf->rf_detect_ptn);
	nzr051s.df_detect_ptn = htol((char *)buf->df_detect_ptn);
	nzr051s.cf_detect_ptn = htol((char *)buf->cf_detect_ptn);
	return( 0 );
}
