/*======================================================================
 * File name    : rzr079.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト２解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr079.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr079.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr079.c 1.2 1996/04/11 17:16:06 hiroki
 *Log: rzr079.c
 * リビジョン 1.2  1996/04/11  17:16:06  hiroki
 * nof_cfを2BYTEに変更。
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
#include	<rzcmd\rzr079.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cf2_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF振幅設定テキスト２を構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cf2_parm(
	struct fmt_cf2_parm	*buf
	)
{
	PR210	*p1;
	P5	*p0;
	int	i, j;

	rzr079s.ctrl      = ascii_hex(buf->ctrl);
	rzr079s.nof_cf    = htoc((char *)buf->nof_cf);
	for(i=0, p1 = (PTR)&buf->cf; i < rzr079s.nof_cf ; i++) {
		rzr079s.cf[i].nof_sig = ascii_hex(p1->nof_sig);
		for(j=0,p0=(PTR)&p1->at;j<rzr079s.cf[i].nof_sig;j++,p0++){
			rzr079s.cf[i].at[j].amp = htoc((char *)&p0->amp);
			rzr079s.cf[i].at[j].time = htoc((char *)&p0->time);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
