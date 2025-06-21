/*======================================================================
 * File name    : rzr119.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	４－２０ｍＡ出力に対するレベル重量スパンテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr119.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr119.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr119.c 1.4 1994/09/12 14:20:39 hiroe
 *Log: rzr119.c
 * リビジョン 1.4  1994/09/12  14:20:39  hiroe
 * 上下限偏差を上下限重量にする。
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
#include	<rzcmd\rzr119.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_dfspan							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	4-20mA出力に対するレベル重量スパンテキストを構造体にセーブする。*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_dfspan(
	struct fmt_dfspan	*buf
	)
{
	BYTE	*p;
	int	i, j;

	rzr119s.min_df_no = ascii_hex( buf->min_df_no );
	rzr119s.nof_df    = ascii_hex( buf->nof_df );
	
	p = (BYTE_PTR)&buf->spanwt;
	
	for(i = rzr119s.nof_df, j = rzr119s.min_df_no-1; i ; i--, j++) {
		rzr119s.spanwt[j].targ = htoi( (char *)p );
		p += sizeof(buf->spanwt[0].targ);
		rzr119s.spanwt[j].upper = htoi( (char *)p );
		p += sizeof(buf->spanwt[0].upper);
		rzr119s.spanwt[j].lower = htoi( (char *)p );
		p += sizeof(buf->spanwt[0].lower);
	}
	return(0);
}
