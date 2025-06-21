/*======================================================================
 * File name    : rzr159.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ＢＨオーバースケール重量テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr159.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr159.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src//RCS/rzr159.c 1.2 1996/05/09 19:19:32 hiroe
 *Log: rzr159.c
 * リビジョン 1.2  1996/05/09  19:19:32  hiroe
 * bh_reject_weight の型を変更する。
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
#include	<rzcmd\rzr159.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_p_reject_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ＢＨオーバースケール重量テキストを構造体にセーブする。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_p_reject_parm(
	struct fmt_p_reject_parm	*buf
	)
{
	PR1591	*p0;
	PR1592	*p1;
	int	i, j;

	rzr159s.ch_id = ascii_hex(buf->ch_id);
	rzr159s.nof_sect = ascii_hex(buf->nof_sect);
	for(i=0, p1=(PTR)&buf->sect; i < rzr159s.nof_sect ; i++) {
		rzr159s.sect[i].nof_phead = ascii_hex(p1->nof_phead);
		for(j=0,p0=(PTR)&p1->phead;j<rzr159s.sect[i].nof_phead;j++,p0++){
			rzr159s.sect[i].phead[j].bh_reject_wt 
					= htol((char *)&p0->bh_reject_wt);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
