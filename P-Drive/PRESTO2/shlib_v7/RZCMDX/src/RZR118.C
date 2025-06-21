/*======================================================================
 * File name    : rzr118.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	 タイミング拡張パラメータテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr118.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr118.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr118.c 1.2 1995/05/15 17:43:11 sakagami
 *Log: rzr118.c
 * リビジョン 1.2  1995/05/15  17:43:11  sakagami
 * サーボ上限停止時間の追加
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
#include	<rzcmd\rzr117.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_rz600_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＲＺ－６００用タイミング構造体データをテキストに変換する。	*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_tmg_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_rz600_tmg(
	struct fmt_rz600_tmg	*txt,
	struct rzr117t		*pkt
	)
{
	ctoh( pkt->cnv_delay, (char *)&txt->cnv_delay );/*搬送ｺﾝﾍﾞﾔ遅れ時間*/
	ctoh( pkt->cnv_hold_tm, (char *)&txt->cnv_hold_tm );/*搬送ｺﾝﾍﾞﾔオン時間*/
	ctoh( pkt->srv_delay, (char *)&txt->srv_delay );/*排出用ｻｰﾎﾞ遅れ時間*/
	ctoh( pkt->srv_hold_tm, (char *)&txt->srv_hold_tm );/*ｻｰﾎﾞ上限停止時間*/
	ctoh( pkt->sv_delay, (char *)&txt->sv_delay );/*排出用ｼﾘﾝﾀﾞ遅れ時間*/
	ctoh( pkt->sv_hold_tm, (char *)&txt->sv_hold_tm );/*排出用ｼﾘﾝﾀﾞオン時間*/
	ctoh( pkt->men1_delay, (char *)&txt->men1_delay );    /*麺押し１遅れ時間*/
	ctoh( pkt->men1_hold_tm, (char *)&txt->men1_hold_tm );/*麺押し１オン時間*/
	ctoh( pkt->men2_delay, (char *)&txt->men2_delay );    /*麺押し２遅れ時間*/
	ctoh( pkt->men2_hold_tm, (char *)&txt->men2_hold_tm );/*麺押し２オン時間*/
	ctoh( pkt->men3_delay, (char *)&txt->men3_delay );    /*麺押し３遅れ時間*/
	ctoh( pkt->men3_hold_tm, (char *)&txt->men3_hold_tm );/*麺押し３オン時間*/
	ctoh( pkt->men4_delay, (char *)&txt->men4_delay );    /*麺押し４遅れ時間*/
	ctoh( pkt->men4_hold_tm, (char *)&txt->men4_hold_tm );/*麺押し４オン時間*/

	return(++txt);
}
