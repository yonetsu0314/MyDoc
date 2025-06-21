/*======================================================================
 * File name    : rzi113a.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	本体状態情報拡張パラメータテキスト解読（拡張１）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzi113a.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi113a.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzi113a.c 1.2 1996/07/26 11:58:20 hiroki
 *Log: rzi113a.c
 * リビジョン 1.2  1996/07/26  11:58:20  hiroki
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
#include	<rzcmd\rzi113a.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_ata_targt						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_ata_target(
	struct fmt_ata_target	*buf
	)
{
	unsigned char	*p;
	int		i;

	rzi113s.nof_ch = ascii_hex(buf->nof_ch);
	/*ここから可変長データ処理*/
	p = (BYTE_PTR)buf->ata_target[0];
	for(i = 0; i < rzi113s.nof_ch; i++) {
		rzi113s.ata_target[i] = htol( (char *)p );
		p += sizeof( buf->ata_target[i] );
	}
	return(0);
}
