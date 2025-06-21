/*======================================================================
 * File name	: prr0135.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	特殊仕様データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/rcs/prr0135.c 1.2 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0135.c $
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 * 2006/11/09 可変長が考慮されていなかった不具合修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2006, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0135.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_sp_parm							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	特殊仕様データテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_sp_parm(
	struct fmt_sp_parm	*buf
	)
{
	BYTE	*p;
	int	i, k;

	prr0135s.ch_id = ascii_hex(buf->ch_id);
	prr0135s.nof_parm = htoc((char *)buf->nof_parm);
	p = (BYTE_PTR)buf->sp_parm;
	for(i = 0; i < prr0135s.nof_parm ; i++) {
		prr0135s.sp_parm[i].spec_no = htoc( (char *)p );
		p += sizeof( buf->sp_parm[i].spec_no );
		prr0135s.sp_parm[i].nof_data = ascii_hex( *p );
		p += sizeof( buf->sp_parm[i].nof_data );
		for(k = 0; k < prr0135s.sp_parm[i].nof_data; k++) {
			prr0135s.sp_parm[i].parm[k] = htol( (char *)p );
			p += sizeof( buf->sp_parm[i].parm[k] );
		}
	}
	return(0);
}
