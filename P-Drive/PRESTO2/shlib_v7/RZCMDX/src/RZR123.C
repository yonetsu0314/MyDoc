/*======================================================================
 * File name    : rzr123.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標拡張パラメータ（１）テキスト解読
 *	優先参加回数
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr123.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr123.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr123.cv  1.2  94/06/03 10:09:00  fukutome
 *Log: p:/lib/rzcmdx/src/rcs/rzr123.cv
 * Revision 1.2  94/06/03 10:09:00  fukutome
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
#include	<rzcmd\rzr123.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_prec_dump						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	優先参加回数テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_prec_dump(
	struct fmt_prec_dump	*buf
	)
{
	unsigned char	*p;
	int		i, n;

	rzr123s.ch_id = ascii_hex(buf->ch_id);
	n = rzr123s.nof_sect = ascii_hex(buf->nof_sect);
	for (i = 0, p = &buf->cnt[0][0]; i < n; ++i) {
		rzr123s.cnt[i] = htoc((char *)p);
		p += sizeof(buf->cnt[0]);
	}
	return 0;
}
