/*======================================================================
 * File name    : rzr020.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給量テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzr020.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr020.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr020.cv  2.1  93/11/02 19:18:12  ryoji
 *Log: p:/lib/rzcmd/src/rcs/rzr020.cv
 * Revision 2.1  93/11/02 19:18:12  ryoji
 * 第５版対応
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
#include	<rzcmd\rzr019.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_rfdf_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	供給量設定構造体のデータをテキストに変換する。			*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_cb_result)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_rfdf_parm(
	struct fmt_rfdf_parm	*txt,
	struct rzr019t		*pkt
	)
{
	BYTE		*buf;
	int		i, j;

	txt->ch_id     = hex_ascii( pkt->ch_id );
	txt->backup    = hex_ascii( pkt->backup );
	txt->afd_mode  = hex_ascii( pkt->afd_mode );
	ctoh( pkt->min_rf_no , (char *)&txt->min_rf_no );
	ctoh( pkt->nof_rf , (char *)&txt->nof_rf );

	buf = (PTR)txt->rf;
	for(i = pkt->nof_rf, j = pkt->min_rf_no-1 ; i ; i--, j++) {
		ctoh(pkt->rf[j].amp, (char *)buf);
		buf += sizeof(txt->rf[0].amp);
		ctoh(pkt->rf[j].time, (char *)buf);
		buf += sizeof(txt->rf[0].time);
	}

	*buf = hex_ascii(pkt->min_df_no);
	buf += sizeof(txt->min_df_no);
	*buf = hex_ascii(pkt->nof_df);
	buf += sizeof(txt->nof_df);

	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		ctoh(pkt->df[j].amp, (char *)buf);
		buf += sizeof(txt->df[0].amp);
		ctoh(pkt->df[j].time, (char *)buf);
		buf += sizeof(txt->df[0].time);
	}

	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		itoh((unsigned short)pkt->lev[j].targ, (char *)buf);
		buf += sizeof(txt->lev[0].targ);
		ctoh(pkt->lev[j].upper, (char *)buf);
		buf += sizeof(txt->lev[0].upper);
		ctoh(pkt->lev[j].lower, (char *)buf);
		buf += sizeof(txt->lev[0].lower);
	}
	return(buf);
}
