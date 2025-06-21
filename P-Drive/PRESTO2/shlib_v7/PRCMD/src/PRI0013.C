/*======================================================================
 * File name	: pri0013.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	状態テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0013.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0013.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/RCS/rzi013a.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzi013a.c
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
#include	<prcmd\pri0013.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_condition						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	コンディションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_condition(
	struct fmt_condition	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0013s.nof_ch = ascii_hex(buf->nof_ch);
	p = (BYTE_PTR)buf->ch;
	for(i = 0 ; i < pri0013s.nof_ch ; i++ ) {
		pri0013s.ch[i].condition = ascii_hex( *p );
		p += sizeof( buf->ch[i].condition );
		pri0013s.ch[i].power_stat = ascii_hex( *p );
		p += sizeof( buf->ch[i].power_stat );
		pri0013s.ch[i].infeed_stat = ascii_hex( *p );
		p += sizeof( buf->ch[i].infeed_stat );
	}
	pri0013s.df_infeed_stat = htoc((char *)p );
	return 0;
}
