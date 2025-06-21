/*======================================================================
 * File name	: pri0029.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0029.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0029.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/nzcmd/src/rcs/nzi029.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: nzi029.c
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/**/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri0029.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	親ヘッド計量結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_pandc_result(
	struct fmt_pandc_result	*buf
	)
{
	BYTE	*p;
	int	i, j;
	char	str[4];

	str[0] = (char)'0';
	pri0029s.ch_id = ascii_hex(buf->ch_id);
	pri0029s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for( i = 0 ; i < pri0029s.nof_sect ; i++) {
		pri0029s.sect[i].nof_phead = ascii_hex( *p );
		p += sizeof( buf->sect[i].nof_phead );
		for( j = 0 ; j < pri0029s.sect[i].nof_phead; j++ ) {
			pri0029s.sect[i].phead[j].weight = (short)htoi( (char *)p );
			p += sizeof(buf->sect[i].phead[j].weight);
			memcpy((PTR)&str[1], (PTR)p, sizeof(buf->sect[i].phead[j].wh_status));
			pri0029s.sect[i].phead[j].wh_status = (short)htoi((char *)str);
			p += sizeof(buf->sect[i].phead[j].wh_status);
			memcpy((PTR)&str[1], (PTR)p, sizeof(buf->sect[i].phead[j].bh_status));
			pri0029s.sect[i].phead[j].bh_status = (short)htoi((char *)str);
			p += sizeof(buf->sect[i].phead[j].bh_status);
		}
	}
	return(0);
}
