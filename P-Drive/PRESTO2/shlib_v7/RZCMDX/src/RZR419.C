/*======================================================================
 * File name	: rzr419.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	補助フィーダ供給量テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr419.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr419.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr419.c 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr419.c
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr419.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_af_parm							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	AF振幅設定テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_af_parm(
	struct fmt_af_parm	*buf
	)
{
	BYTE	*p;
	int	i, j;

	rzr419s.min_af_no = htoc( (char *)buf->min_af_no );
	rzr419s.nof_af    = htoc( (char *)buf->nof_af );

	p = (BYTE_PTR)&buf->af;
	for(i = rzr419s.nof_af, j = rzr419s.min_af_no-1 ; i ; i--, j++) {
		rzr419s.af[j].amp = htoc( (char *)p );
		p += sizeof(buf->af[0].amp);
		rzr419s.af[j].time = htoc( (char *)p );
		p += sizeof(buf->af[0].time);
	}
	return(0);
}
