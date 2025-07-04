/*======================================================================
 * File name    : sigsem.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/sigsem.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sigsem.c $
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
/*	SIG_SEM								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	セマフォ資源の返却。セマフォの返却資源数のだけシステムコールを	*/
/*	発行する。システムコールの戻り値（エラーコード）をコンバートし	*/
/*	て返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	semaa	: セマフォのアクセスアドレス				*/
/*	cnt	: セマフォの操作する資源数				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
SIG_SEM(
	unsigned int 	semaa,
	unsigned int	cnt
	)
{
	ER		err;
	for(; cnt; cnt--) {
		err = sig_sem( (ID)semaa);
		if(err < E_OK)			return( err_code( err));
	}
	return( err_code( err));
}
