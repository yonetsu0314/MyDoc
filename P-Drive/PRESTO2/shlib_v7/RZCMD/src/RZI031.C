/*======================================================================
 * File name    : rzi031.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzi031.c 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: rzi031.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2003/05/30  14:42
 * キャストの方法が間違っていたので修正する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/04/30  09:49
 * 左辺のキャストを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi031.cv  1.2  94/04/12 15:53:56  kabumoto
 *Log: P:/LIB/RZCMD/SRC/RCS/rzi031.cv
 * Revision 1.2  94/04/12 15:53:56  kabumoto
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
#include	<rzcmd\rzi031.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_cf_rslt							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	CF制御結果テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_cf_rslt(
	struct fmt_cf_rslt	*buf
	)
{
	PI310		*p1;
	P5		*p0;
	BYTE		*p2;
	int		i, j;

	rzi031s.seq_no    = ascii_hex(buf->seq_no);
	rzi031s.nof_cf    = ascii_hex(buf->nof_cf);
	for(i=0, p1 = (PTR)&buf->cf; i < rzi031s.nof_cf ; i++) {
		rzi031s.cf[i].nof_sig = ascii_hex(p1->nof_sig);
		for(j=0,p0=(PTR)&p1->at;j<rzi031s.cf[i].nof_sig;j++,p0++){
			rzi031s.cf[i].at[j].amp = htoc((char *)&p0->amp);
			rzi031s.cf[i].at[j].time = htoc((char *)&p0->time);
		}
		p1 = (PTR)p0;
		rzi031s.cf[i].sig_stat = htoc((char *)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->sig_stat);
		p1 = (PI310 *)p2;
		rzi031s.cf[i].df_wt = (short)htoi((char *)p1);
		p2 = (BYTE_PTR)p1;
		p2 += sizeof(p1->df_wt);
		p1 = (PI310 *)p2;
	}
	return(0);
}
