/*----------------------------------------------------------------------*
 * File name	: dadsck.c	/ test self check			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadsck.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadsck.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dadsck.c 1.1 1996/07/22 13:37:16 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｾﾙﾌ･ﾁｪｯｸの完了をﾃｽﾄする。完了時はslf_chkへのﾎﾟｲﾝﾀを返す。	*/
/*	注:ｾﾙﾌ･ﾁｪｯｸの完了はDADCのｼｽﾃﾑが初期状態から通常状態に遷移	*/
/*	   した事を表す。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ﾎﾞｰﾄﾞ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	far *		: &dad_slf_chk	･･･ 完了			*/
/*			  NULL		･･･ 未完了			*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	"dadctl.h"			/* Show current		*/

DADSCK *
dad_self_chk(
	DAD_ID		pwb
	)
{
	if((dad_slf_chk[pwb].ref0)
	  |(dad_slf_chk[pwb].ref1))		return(&dad_slf_chk[pwb]);
	return(NULL);
}
