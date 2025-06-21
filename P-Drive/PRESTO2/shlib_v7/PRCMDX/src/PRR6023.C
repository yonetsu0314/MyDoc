/*======================================================================
 * File name	: prr6023.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	セミオート機計量データテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr6023.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr6023.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/12/06  15:35
 * ＢＨ非同期供給のON/OFFを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr6023.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_semi_auto						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	セミオート機データテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_semi_auto(
	struct fmt_semi_auto	*buf
	)
{
	BYTE	*p;
	int	i;

	prr6023s.ch_id = ascii_hex(buf->ch_id);
	prr6023s.dump_dir = ascii_hex(buf->dump_dir);
	prr6023s.conv_rev = ascii_hex(buf->conv_rev);
	prr6023s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ;i < prr6023s.nof_sect ;i++ ) {
		prr6023s.sect[i].wh_async_supply = ascii_hex( *p );
		p += sizeof(buf->sect[i].wh_async_supply);
		prr6023s.sect[i].only_bh_cal = ascii_hex( *p );
		p += sizeof(buf->sect[i].only_bh_cal);
		prr6023s.sect[i].bh_async_supply = ascii_hex( *p );
		p += sizeof(buf->sect[i].bh_async_supply);
	}
	return(0);
}
