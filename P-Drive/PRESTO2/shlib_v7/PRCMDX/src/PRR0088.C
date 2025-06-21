/*======================================================================
 * File name    : prr0088.c
 * Original	: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr088.c 1.1
 *----------------------------------------------------------------------
 * Function	:
 *	PLC書込みデータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr0088.c 1.1 1970/01/01 00:00:00 ishidat Exp $
 *$Log: prr0088.c $
 * リビジョン 1.1  1970/01/01  00:00:00  ishidat
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr088.c 1.2 1997/11/18 17:16:06 chimura
 *Log: rzr088.c
 * リビジョン 1.2  1997/11/18  17:16:06  chimura
 * rzr087.hの変更に伴う修正
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
#include	<prcmd\prr0087.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_plc_data						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	PLC書込みデータをテキストに変換する。				*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt				･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_plc_data(
	struct fmt_plc_wr	*txt,
	struct prr0087t		*pkt
	)
{
	int	i;
	for(i=0; i<25; ++i){
		itoh( (unsigned short)pkt->plc_wr_data[i],(char *)&txt->plc_wr_data[i][0]);
	}
	itoh( (unsigned short)pkt->dump_mode,(char *)&txt->dump_mode[0]);
	return(++txt);
}
