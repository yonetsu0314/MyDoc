/*----------------------------------------------------------------------
 * File name	: rtcgtm.c	/ rtc get time
 * Original	: p:/lib/drive/rtc/rcs/rtcgtm.c 1.2
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcgtm.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rtcgtm.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * リアルタイム・クロック
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA SCALES MFG. CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *----------------------------------------------------------------------
 * 説明
 *	RTCから時刻を読み出す
 *----------------------------------------------------------------------
 *  引き数
 *	int	*hour	: 時へのポインタ
 *	int	*min	: 分へのポインタ
 *	int	*sec	: 秒へのポインタ
 *
 * 戻り値
 *	int		: 0	･･･ success
 *			  -1･･･ error code
 *----------------------------------------------------------------------
 */
#include	<limits.h>				/*	"		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	<drive\rtcdrv.h>
#include	"rtcctl.h"

int
rtc_gtime(
	int			*hour,
	int			*min,
	int			*sec
	)
{
	int				err = 0,err1;

	ADV_WAI_SEM(err, rtc_nest, ENDLESS,rtc_sem.acadr, VOID_TIME);
	if(err)	return(err|INT_MIN);
	err1 = _rtc_get_time(hour, min, sec);
	ADV_SIG_SEM(err, rtc_nest, rtc_sem.acadr);
	if(err)	return(err|INT_MIN);
	return(err1);
}
