/*======================================================================
 * File name    : waiflg.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/waiflg.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: waiflg.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
*/
#include	<rxif\rx116.h>			/* Show include		*/
#include	<kernel.h>
#include	"errcode.h"


/*----------------------------------------------------------------------*/
/*	WAI_FLG								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	イベントフラグ待ち。イベントフラグの待ち方の指定により、イベン	*/
/*	トフラグの待ちモードをセットし、タイムアップ有り無しによりシス	*/
/*	テムコールを選択し、実行する。システムコールの戻り値（エラーコ	*/
/*	ード）をコンバートして返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	opt	: イベントフラグの待ち方法の指定			*/
/*	*flg	: イベントフラグの内容を返すエリアへのポインタ		*/
/*	flgaa	: イベントフラグのアクセスアドレス			*/
/*	ptn	: イベントフラグの待ちビットパターン			*/
/*	*tm	: イベントフラグの待ち時間				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
WAI_FLG(
	unsigned int	opt,
	unsigned int 	*flg,
	unsigned int	flgaa,
	unsigned int	ptn,
	long  		*tm
	)
{
	FLGPTN		waiptn = ptn;
	UINT		wfmode;
	if(opt & EVF_OR)	wfmode = TWF_ORW;
	else			wfmode = TWF_ANDW;
	if(opt & EVF_RES)	wfmode |= TWF_CLR;
	if(opt & TIME_OUT)
		return( err_code( twai_flg( (ID)flgaa, waiptn, wfmode,
			(UINT *)flg, (TMO)*tm)));
	else
		return( err_code( wai_flg( (ID)flgaa, waiptn, wfmode,
			(UINT *)flg)));
}
