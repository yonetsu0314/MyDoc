/*======================================================================
 * File name    : rzr063.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	上下動シュートパラメータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr063.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr063.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr063.c 1.2 1995/05/02 14:14:52 hiroe
 *Log: rzr063.c
 * リビジョン 1.2  1995/05/02  14:14:52  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom/typedefs.h>		/* Show xlib		*/
#include	<custom/custom.h>		/*	"		*/
#include	<rzcmd/rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd/rzr063.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_updown_shoot						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	上下動シュートテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_updown_shoot(
	struct updown_shoot_parm	*buf
	)
{
	unsigned char	*p;

	p = buf->dev_id;
	rzr063s.dev_id = htoc((char *)p);
	p += sizeof(buf->dev_id);
	rzr063s.nof_pos = htoc((char *)p);
	p += sizeof(buf->nof_pos);
	rzr063s.err_detect_interval = htoc((char *)p);
	p += sizeof(buf->err_detect_interval);
	rzr063s.position = htoc((char *)p);
	return 0;
}
