/*======================================================================
 * File name    : rzr259.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張親ヘッドデータテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr259.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr259.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr259.c 1.3 1999/05/06 10:16:29 itami
 *Log: rzr259.c
 * リビジョン 1.3  1999/05/06  10:16:29  itami
 * WH->BH非同期供給設定追加
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
#include	<rzcmd\rzr259.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_pandc_ext_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張親ヘッドデータテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/
int
rz_scan_pandc_ext_parm(
	struct fmt_pandc_ext_parm	*buf
	)
{
	PR2590		*p0;
	PR2591		*p1;
	int		i, j;

	rzr259s.ch_id = ascii_hex(buf->ch_id);
	rzr259s.nof_sect = ascii_hex(buf->nof_sect);
	for(i=0, p1=(PTR)&buf->sect; i < rzr259s.nof_sect ; i++) {
		rzr259s.sect[i].nof_phead = ascii_hex(p1->nof_phead);
		rzr259s.sect[i].only_parent = ascii_hex(p1->only_parent);
		for(j=0,p0=(PTR)&p1->phead;j<rzr259s.sect[i].nof_phead;j++,p0++) {
			rzr259s.sect[i].phead[j].fd_whbh = ascii_hex(p0->fd_whbh);
			rzr259s.sect[i].phead[j].phfd_on = htoc((char *)&p0->phfd_on);
			rzr259s.sect[i].phead[j].ph_cv = htoc((char *)&p0->ph_cv);
			rzr259s.sect[i].phead[j].phcv_on = htoc((char *)&p0->phcv_on);
		}
		p1 = (PTR)p0;
	}
	return(0);
}
