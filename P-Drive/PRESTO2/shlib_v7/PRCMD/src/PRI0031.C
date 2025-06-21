/*======================================================================
 * File name	: pri0031.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0031.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0031.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi031.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi031.c
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
#include	<prcmd\pri0031.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_cf_rslt							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	CF制御結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_cf_rslt(
	struct fmt_cf_rslt	*buf
	)
{
	BYTE	*p;
	int	i, j;

	pri0031s.seq_no = ascii_hex(buf->seq_no);
	pri0031s.nof_cf = ascii_hex(buf->nof_cf);
	p = (BYTE_PTR)buf->cf;
	for( i = 0 ; i < pri0031s.nof_cf ; i++) {
		pri0031s.cf[i].nof_sig = ascii_hex( *p );
		p += sizeof( buf->cf[i].nof_sig );
		for( j = 0 ; j < pri0031s.cf[i].nof_sig; j++ ) {
			pri0031s.cf[i].at[j].amp = htoc((char *)p );
			p += sizeof( buf->cf[i].at[j].amp );
			pri0031s.cf[i].at[j].time = htoc((char *)p );
			p += sizeof( buf->cf[i].at[j].time );
		}
		pri0031s.cf[i].sig_stat = htoc( (char *)p );
		p += sizeof(buf->cf[i].sig_stat);
		pri0031s.cf[i].df_wt = (short)htoi( (char *)p );
		p += sizeof(buf->cf[i].df_wt);
	}
	return(0);
}
