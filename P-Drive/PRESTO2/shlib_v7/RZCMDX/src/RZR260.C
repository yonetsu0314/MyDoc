/*======================================================================
 * File name    : rzr260.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張親ヘッドデータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr260.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr260.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr260.c 1.2 1999/05/06 10:16:51 itami
 *Log: rzr260.c
 * リビジョン 1.2  1999/05/06  10:16:51  itami
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
#include	<rzcmd\rzr259.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_pandc_ext_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	拡張親ヘッドデータテキストの構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_pandc_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_pandc_ext_parm(
	struct fmt_pandc_ext_parm	*txt,
	struct rzr259t		*pkt
	)
{
	BYTE		*buf;
	RZR2591T	*p1;
	RZR2590T	*p0;
	int		i, j;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i=0, p1=(PTR)&pkt->sect; i < pkt->nof_sect ; i++, p1++) {
		*buf = hex_ascii(p1->nof_phead);
		buf += sizeof(txt->sect[i].nof_phead);
		*buf = hex_ascii(p1->only_parent);
		buf += sizeof(txt->sect[i].only_parent);
		for(j=0, p0=(PTR)&p1->phead; j < p1->nof_phead; j++, p0++) {
			*buf = hex_ascii(p0->fd_whbh);
			buf += sizeof(txt->sect[i].phead[j].fd_whbh);
			ctoh(p0->phfd_on, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].phfd_on);
			ctoh(p0->ph_cv, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].ph_cv);
			ctoh(p0->phcv_on, (char *)buf);
			buf += sizeof(txt->sect[i].phead[j].phcv_on);
		}
	}
	return(buf);
}
