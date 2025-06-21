/*======================================================================
 * File name    : prr3017.c
 * Original	: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr317.c 1.1
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングテキスト解読
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/shlib_v7/PRCMDX/src/RCS/prr3017.c 1.1 2015-05-28 12:01:03+09 itami Exp $
 *$Log: prr3017.c $
 *Revision 1.1  2015-05-28 12:01:03+09  itami
 *Initial revision
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr3017.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_round_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	WH用のラウンディングテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_round_tmg(
	struct fmt_round_tmg	*buf
	)
{
	int	i;

	prr3017s.nof_round    = ascii_hex( buf->nof_round );
	for(i = 0; i < prr3017s.nof_round; i++) {
		prr3017s.round_base[i] = htoc( (char *)buf->round_base[i] );
	}
	return(0);
}
