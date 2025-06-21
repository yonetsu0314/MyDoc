/*======================================================================
 * File name    : nzr009.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	安定判定時間 拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzr009.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr009.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/nzcmd/src/rcs/nzr009.c 1.1 1999/06/17 10:11:28 kawamura
 *Log: nzr009.c
 * リビジョン 1.1  1999/06/17  10:11:28  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<nzcmd\nzcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<nzcmd\nzr009.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	nz_scan_wei_spec1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	安定判定時間 拡張パラメータテキストを構造体にセーブする。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
nz_scan_wei_spec1(
	struct fmt_wei_spec1_nz	*buf
	)
{
	NR0091	*p1;
	NR0090	*p0;
	int	i,j;

	nzr009s.nof_ch = ascii_hex(buf->nof_ch);
	for(i=0, p1 = (PTR)&buf->ch; i < nzr009s.nof_ch ; i++) {
		nzr009s.ch[i].nof_sect = ascii_hex(p1->nof_sect);
		for(j=0,p0=(PTR)&p1->sect;j<nzr009s.ch[i].nof_sect;j++,p0++){
			nzr009s.ch[i].sect[j].stable_time = (unsigned short)htoi( (char *)p0->stable_time );
		}
		p1 = (PTR)p0;
	}
	return(0);
}
