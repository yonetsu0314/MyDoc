/*======================================================================
 * File name	: prr0019.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給量テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0019.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0019.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzr019.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr019.c
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
#include	<prcmd\prr0019.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_rfdf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	RF&DF振幅設定テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_rfdf_parm(
	struct fmt_rfdf_parm	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0019s.backup = ascii_hex( buf->backup );
	prr0019s.nof_rf = htoc( (char *)buf->nof_rf );

	p = (BYTE_PTR)buf->rf;
	for( i = 0; i < (int)prr0019s.nof_rf; i++ ) {
		prr0019s.rf[i].amp = htoc( (char *)p );
		p += sizeof(buf->rf[i].amp);
		prr0019s.rf[i].time = htoc( (char *)p );
		p += sizeof(buf->rf[i].time);
	}

	prr0019s.nof_df = ascii_hex( *p );
	p += sizeof(buf->nof_df);
	for( i = 0; i < (int)prr0019s.nof_df; i++ ) {
		prr0019s.df[i].amp = htoc( (char *)p );
		p += sizeof(buf->df[i].amp);
		prr0019s.df[i].time = htoc( (char *)p );
		p += sizeof(buf->df[i].time);
	}

	prr0019s.nof_af = htoc( (char *)p );
	p += sizeof(buf->nof_af);
	for( i = 0; i < (int)prr0019s.nof_af; i++ ) {
		prr0019s.af[i].amp = htoc( (char *)p );
		p += sizeof(buf->af[i].amp);
		prr0019s.af[i].time = htoc( (char *)p );
		p += sizeof(buf->af[i].time);
	}
	return(0);
}
