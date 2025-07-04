/*======================================================================
 * File name    : canwup.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/canwup.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: canwup.c $
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
/*	CAN_WUP								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	タスク起床要求のキャンセル。キューイングされていた起床要求回数	*/
/*	をシステムコールの戻り値からポインタ渡しにコンバータする。また、*/
/*	システムコールの戻り値がマイナスの場合は、エラーコードとなり、	*/
/*	変換後返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*cnt	: キューイングされていた起床要求回数を示すポインタ	*/
/*	tskaa	: タスクのアクセスアドレス				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
CAN_WUP(
	unsigned int 	*cnt,
	unsigned int	tskaa
	)
{

/*	(ER_UNIT)*cnt = can_wup( (ID)tskaa); */
	*cnt = can_wup( (ID)tskaa);
	if(*cnt < E_OK)	return( err_code( (ER)*cnt));

	return(E_OK);
}
