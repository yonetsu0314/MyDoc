/*======================================================================
 * File name    : rzr319.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	セクション毎のAFD設定テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr319.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr319.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr319.c 1.4 1998/03/04 09:13:05 hiroe
 *Log: rzr319.c
 * リビジョン 1.4  1998/03/04  09:13:05  hiroe
 * buf->nof_sect -> rzr319s.nof_sectにする。
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
#include	<rzcmd\rzr319.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_sect_afd_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	セクション毎のAFD設定テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_sect_afd_parm(
	struct fmt_sect_afd_parm	*buf
	)
{
	int	i;

	rzr319s.ch_id     = ascii_hex( buf->ch_id );
	rzr319s.nof_sect  = ascii_hex( buf->nof_sect );
	for(i = 0; i < rzr319s.nof_sect; i++) {
		rzr319s.afd_mode[i]  = ascii_hex( buf->afd_mode[i][0] );
	}
	return(0);
}
