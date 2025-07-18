/*----------------------------------------------------------------------*
 * File name	: cntint.c	   											*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/rsh/RCS/cntint.c 1.2 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cntint.c $
 * リビジョン 1.2  1970/01/01  00:00:00  yonetsu
 * 2001.01.08 15:24
 * BIOS の INCLUDE フォルダーの変更.
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		リフレッシュコントローラを使用したカウンター					*/
/*----------------------------------------------------------------------*/
#include	<bios2357\h8sbsc.h>
#include	"cnt.h"
/*----------------------------------------------------------------------*/
/* カウンターの為の割り込み関数											*/
/*----------------------------------------------------------------------*/
#pragma interrupt(counter_int)
void counter_int( void )
{
	int *c = syscnt.cnt;
	int *e = syscnt.enable;
	int i;
	
	BSC.DRAMCR.BIT.CMF = 0;			/* IREQ Reset		*/
	if(syscnt.enable_flag) {
		for(i=0 ;i<syscnt.enable_flag ;i++ ,c++ ,e++) {
			if(*e)	(*c)++;
		}
	}
}
