/*======================================================================
 * File name    : rzr091.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *　マルチ連動パラメータ1テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr091.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr091.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr091.c 1.3 1997/12/08 21:39:27 itami
 *Log: rzr091.c
 * リビジョン 1.3  1997/12/08  21:39:27  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr091.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/* 	rz_scan_mlt_parm1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動パラメータ1のテキストを構造体にセーブする。    　　	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_mlt_parm1 (
	struct fmt_mlt_parm1	*buf
	)
{
	unsigned short	d;

	rzr091s.ch_id		= ascii_hex( buf->ch_id);
	rzr091s.neighbor_node	= htoc( (char *)buf->neighbor_node);
	rzr091s.shift_cnt	= htoc( (char *)buf->shift_cnt);
	rzr091s.retry_time    	= htoc( (char *)buf->retry_time);
	rzr091s.retry_span    	= htoc( (char *)buf->retry_span);
	rzr091s.retry_cnt    	= htoc( (char *)buf->retry_cnt);
	rzr091s.master_slave    = htoc( (char *)buf->master_slave);
	rzr091s.ch_status    	= htoc( (char *)buf->ch_status);
/*	*((unsigned short *)&rzr091s.dsw) = htoi( (char *)buf->dsw);	*/
/*	処理系によるビットフィールドの違いを無理矢理あわせる		*/
	d = htoi( (char *)buf->dsw);
	rzr091s.dsw.my_node = d & 0x001f;
	rzr091s.dsw.chg_ch = (d & 0x0020) >> 5;
	rzr091s.dsw.ilk_inf = (d & 0x00c0) >> 6;
	rzr091s.dsw.Dummy = (d & 0xff00) >> 8;
	return(0);
}
