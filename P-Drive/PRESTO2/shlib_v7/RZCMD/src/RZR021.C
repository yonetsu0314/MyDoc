/*======================================================================
 * File name    : rzr021.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr021.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr021.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr021.cv  2.3  94/04/12 15:53:48  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzr021.cv
 * Revision 2.3  94/04/12 15:53:48  kabumoto
 * ワーニング除去
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<rzcmd\rzr021.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cf_parm							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF振幅設定テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cf_parm(
	struct fmt_cf_parm	*buf
	)
{
	PR210		*p1;
	P5		*p0;
	int		i, j;

	rzr021s.ctrl      = ascii_hex(buf->ctrl);
	rzr021s.nof_cf    = ascii_hex(buf->nof_cf);
	for(i=0, p1 = (PTR)&buf->cf; i < rzr021s.nof_cf ; i++) {
		rzr021s.cf[i].nof_sig = ascii_hex(p1->nof_sig);
		for(j=0,p0=(PTR)&p1->at;j<rzr021s.cf[i].nof_sig;j++,p0++){
			rzr021s.cf[i].at[j].amp = htoc((char *)&p0->amp);
			rzr021s.cf[i].at[j].time = htoc((char *)&p0->time);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
