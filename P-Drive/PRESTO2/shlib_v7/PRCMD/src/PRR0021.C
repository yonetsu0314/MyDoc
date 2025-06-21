/*======================================================================
 * File name	: prr0021.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0021.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0021.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzr021.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr021.c
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
#include	<prcmd\prr0021.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_cf_parm							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	CF振幅設定テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_cf_parm(
	struct fmt_cf_parm	*buf
	)
{
	BYTE	*p;
	int	i,j;

	prr0021s.ctrl = ascii_hex( buf->ctrl );
	prr0021s.nof_cf = ascii_hex( buf->nof_cf );

	p = (BYTE_PTR)buf->cf;
	for(i = 0 ; i < prr0021s.nof_cf ; i++) {
		prr0021s.cf[i].nof_sig = ascii_hex( *p );
		p += sizeof( buf->cf[i].nof_sig );
		for(j = 0; j < prr0021s.cf[i].nof_sig ; j++ ) {
			prr0021s.cf[i].at[j].amp = htoc( (char *)p );
			p += sizeof( buf->cf[i].at[j].amp );
			prr0021s.cf[i].at[j].time = htoc( (char *)p );
			p += sizeof( buf->cf[i].at[j].time );
		}
	}
	return(0);
}
