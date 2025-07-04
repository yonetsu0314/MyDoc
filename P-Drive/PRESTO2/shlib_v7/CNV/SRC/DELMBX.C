/*======================================================================
 * File name    : delmbx.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/delmbx.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: delmbx.c $
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
/*	DEL_MBX								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	メールボックスの削除。メールボックスのアセスアドレスをイベント	*/
/*	フラグＩＤとしてシステムコールを行う。システムコールの戻り値（	*/
/*	エラーコード）をコンバートして返す。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	mbxaa	: メールボックスのアクセスアドレス			*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
DEL_MBX(
	unsigned int	mbxaa
	)
{
	return( err_code( del_mbx( (ID)mbxaa)));
}
