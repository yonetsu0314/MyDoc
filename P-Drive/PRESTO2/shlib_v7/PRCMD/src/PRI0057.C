/*======================================================================
 * File name	: pri0057.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ホッパ駆動回数データ応答テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/pri0057.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0057.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<string.h>			/* ANSI std		*/
#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\pri0057.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_hop_drive_no						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ホッパ駆動回数データ応答テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_hop_drive_no(
	struct fmt_hop_drive_no	*buf
	)
{
	BYTE	*p;
	int	i;

	pri0057s.hopper_id = ascii_hex(buf->hopper_id);
	pri0057s.nof_hopper = htoc((char *)buf->nof_hopper);
	p = (BYTE_PTR)buf->nof_times[0];
	for( i = 0 ; i < pri0057s.nof_hopper ; i++ ) {
		pri0057s.nof_times[i] = htol((char *)p );
		p += sizeof(buf->nof_times[i]);
	}
	return 0;
}
