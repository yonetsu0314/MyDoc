/*----------------------------------------------------------------------*
 * File name	: cntdis.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/rsh/RCS/cntdis.c 1.2 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cntdis.c $
 * リビジョン 1.2  1970/01/01  00:00:00  yonetsu
 * 2001.01.01  15:23
 * BIOS の INCLUDE フォルダーの変更
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
#include	<custom\custom.h>
#include	<drive\cntdrv.h>
#include	"cnt.h"

/*----------------------------------------------------------------------*/
/* カウンターを使用可能にする											*/
/*																		*/
/* 引き数																*/
/*		counter_no :	カウンターの番号 ( 1 - 初期化時のカウンターの数	*/
/* 戻り値																*/
/*		0  :  成功														*/
/*		-1 :  失敗														*/
/*----------------------------------------------------------------------*/
int dis_scnt(
	int	counter_no
	)
{
	if(syscnt.enable_flag == 0)				return -1;
	if(counter_no > syscnt.enable_flag)		return -1;
	counter_no--;

	_disable();
	*(syscnt.enable + counter_no) = 0;
	_enable();

	return 0;
}
