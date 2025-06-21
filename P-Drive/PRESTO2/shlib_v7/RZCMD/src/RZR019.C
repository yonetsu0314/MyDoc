/*======================================================================
 * File name    : rzr019.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給量テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr019.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzr019.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:49
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmd/src/rcs/rzr019.c 2.1 1993/11/02 19:18:22 ryoji
 *Log: rzr019.c
 * リビジョン 2.1  1993/11/02  19:18:22  ryoji
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
#include	<rzcmd\rzr019.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_rfdf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	RF&DF振幅設定テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_rfdf_parm(
	struct fmt_rfdf_parm	*buf
	)
{
	BYTE		*p;
	int		i, j;

	rzr019s.ch_id     = ascii_hex( buf->ch_id );
	rzr019s.backup    = ascii_hex( buf->backup );
	rzr019s.afd_mode  = ascii_hex( buf->afd_mode );
	rzr019s.min_rf_no = htoc((char *) &buf->min_rf_no );
	rzr019s.nof_rf    = htoc((char *) &buf->nof_rf );

	p = (PTR)&buf->rf;
	for(i = rzr019s.nof_rf, j = rzr019s.min_rf_no-1 ; i ; i--, j++) {
		rzr019s.rf[j].amp = htoc( (char *)p );
		p += sizeof(buf->rf[0].amp);
		rzr019s.rf[j].time = htoc( (char *)p );
		p += sizeof(buf->rf[0].time);
	}

	rzr019s.min_df_no = ascii_hex( *p );
	p += sizeof(buf->min_df_no);
	rzr019s.nof_df    = ascii_hex( *p );
	p += sizeof(buf->nof_df);

	for(i = rzr019s.nof_df, j = rzr019s.min_df_no-1 ; i ; i--, j++) {
		rzr019s.df[j].amp = htoc( (char *)p );
		p += sizeof(buf->df[0].amp);
		rzr019s.df[j].time = htoc( (char *)p );
		p += sizeof(buf->df[0].time);
	}

	for(i = rzr019s.nof_df, j = rzr019s.min_df_no-1 ; i ; i--, j++) {
		rzr019s.lev[j].targ = (unsigned short)htoi( (char *)p );
		p += sizeof(buf->lev[0].targ);
		rzr019s.lev[j].upper = htoc( (char *)p );
		p += sizeof(buf->lev[0].upper);
		rzr019s.lev[j].lower = htoc( (char *)p );
		p += sizeof(buf->lev[0].lower);
	}
	return(0);
}
