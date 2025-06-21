/*======================================================================
 * File name    : dzi204.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ係数テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzi204.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzi204.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZI204.Cv  1.2  94/02/18 16:43:36  ryoji
 *Log: P:/LIB/DZCMD/SRC/RCS/DZI204.Cv
 * Revision 1.2  94/02/18 16:43:36  ryoji
 * 第１版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<dzcmd\dzcmd.h>			/* Show Current		*/
#include	<dzcmd\dzi203.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	dz_print_afv_coeffi						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＡＦＶ係数データをテキストに変換する。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_wei_spec)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
dz_print_afv_coeffi(
	struct fmt_afv_coeffi	*txt,
	struct dzi203t		*pkt
	)
{
	ctoh( (unsigned char)pkt->wh_no, (char *)&txt->wh_no );
	ltoh( (unsigned long)pkt->new_coeffi, (char *)&txt->new_coeffi );
	ltoh( (unsigned long)pkt->old_coeffi, (char *)&txt->old_coeffi );
	return(++txt);
}
