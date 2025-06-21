/*======================================================================
 * File name    : rzr151.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張テスト駆動開始テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr151.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr151.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr151.c 1.1 1970/01/01 00:00:00 itami
 *Log: rzr151.c
 * リビジョン 1.1  1970/01/01  00:00:00  itami
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
#define		STORAGE
#include	<rzcmd\rzr151.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_ext_tstdrv_run						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張テスト駆動開始テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_ext_tstdrv_run(
	struct fmt_ext_tstdrv_run	*buf
	)
{
	rzr151s.sht_ptn = htol((char *)&buf->sht_ptn);
	rzr151s.sht_flg = ascii_hex(buf->sht_flg);
	rzr151s.spa_ptn = htol((char *)&buf->spa_ptn);
	rzr151s.spa_flg = ascii_hex(buf->spa_flg);

	return(0);
}
