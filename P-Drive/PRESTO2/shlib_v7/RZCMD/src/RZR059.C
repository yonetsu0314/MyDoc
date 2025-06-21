/*======================================================================
 * File name    : rzr059.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr059.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzr059.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:39
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:50
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr059.cv  1.2  94/04/12 16:02:24  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzr059.cv
 * Revision 1.2  94/04/12 16:02:24  kabumoto
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
#include	<rzcmd\rzr059.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	rz_scan_pandc_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	親ヘッド計量目標値テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_pandc_parm(
	struct fmt_pandc_parm	*buf
	)
{
	PR591		*p0;
	PR592		*p1;
	BYTE		*p2;
	int		i, j;

	rzr059s.ch_id = ascii_hex(buf->ch_id);
	rzr059s.nof_head = htoc((char *)&buf->nof_head);
	rzr059s.nof_sect = ascii_hex(buf->nof_sect);
	for(i=0, p1=(PTR)&buf->sect; i < rzr059s.nof_sect ; i++) {
		rzr059s.sect[i].nof_phead = ascii_hex(p1->nof_phead);
		for(j=0,p0=(PTR)&p1->phead;j<rzr059s.sect[i].nof_phead;j++,p0++){
			rzr059s.sect[i].phead[j].no = htoc((char *)&p0->no);
			rzr059s.sect[i].phead[j].upper_wt=(unsigned short)htoi((char *)&p0->upper_wt);
			rzr059s.sect[i].phead[j].lower_wt=(unsigned short)htoi((char *)&p0->lower_wt);
			rzr059s.sect[i].phead[j].rf_ctrl = ascii_hex(p0->rf_ctrl);
			rzr059s.sect[i].phead[j].ph_fd = htoc((char *)&p0->ph_fd);
		}
		p1 = (PTR)p0;
		rzr059s.sect[i].on_off = ascii_hex(*(BYTE_PTR)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->on_off);
		p1 = (PR592 *)p2;
		rzr059s.sect[i].dump = ascii_hex(*(BYTE_PTR)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->dump);
		p1 = (PR592 *)p2;
		rzr059s.sect[i].calc = ascii_hex(*(BYTE_PTR)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->calc);
		p1 = (PR592 *)p2;
		rzr059s.sect[i].az_intvl = (unsigned short)htoi((char *)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->az_intvl);
		p1 = (PR592 *)p2;
		rzr059s.sect[i].over = ascii_hex(*(BYTE_PTR)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->over);
		p1 = (PR592 *)p2;
		rzr059s.sect[i].under = ascii_hex(*(BYTE_PTR)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->under);
		p1 = (PR592 *)p2;
	}
	return(0);
}
