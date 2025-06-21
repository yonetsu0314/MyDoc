/*======================================================================
 * File name    : rzi029.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi029.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi029.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi029.cv  1.2  94/04/12 15:53:58  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi029.cv
 * Revision 1.2  94/04/12 15:53:58  kabumoto
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
#include	<rzcmd\rzi029.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_pandc_result						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_pandc_result(
	struct fmt_pandc_result	*buf
	)
{
	PI291		*p0;
	PI292		*p1;
	int		i, j;

	rzi029s.ch_id = ascii_hex(buf->ch_id);
	rzi029s.nof_head = htoc((char *)&buf->nof_head);
	rzi029s.nof_sect = ascii_hex(buf->nof_sect);
	for(i=0, p1 = (PTR)&buf->sect; i < rzi029s.nof_sect ; i++) {
		rzi029s.sect[i].nof_phead = ascii_hex(p1->nof_phead);
		for(j=0,p0=(PTR)&p1->phead;j<rzi029s.sect[i].nof_phead;j++,p0++){
			rzi029s.sect[i].phead[j].weight = (short)htoi((char *)&p0->weight);
			rzi029s.sect[i].phead[j].wh_status = (unsigned char)htoc((char *)&p0->wh_status);
			rzi029s.sect[i].phead[j].bh_status = (unsigned char)htoc((char *)&p0->bh_status);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
