/*======================================================================
 * File name	: prr0137.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	拡張パラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/rcs/prr0137.c 1.1 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0137.c $
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2007, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr0137.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_ext_parm						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	拡張パラメータテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_ext_parm(
	struct fmt_ext_parm	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0137s.ch_id = ascii_hex(buf->ch_id);
	prr0137s.sub_cmd = htoc((char *)buf->sub_cmd);
	prr0137s.nof_parm = htoc((char *)buf->nof_parm);
	p = (BYTE_PTR)buf->parm;
	for(i = 0; i < prr0137s.nof_parm ; i++) {
		prr0137s.parm[i] = htol( (char *)p );
		p += sizeof( buf->parm[i] );
	}
	return 0;
}
