/*----------------------------------------------------------------------
 * File name	: rtcstm.c	/ rtc set time
 * Original	: p:/lib/drive/rtc/rcs/rtcstm.c 1.2
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcstm.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rtcstm.c $
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
 *	RTCに時刻をセットする
 *----------------------------------------------------------------------
 *  引き数
 *	int	hour	: 時
 *	int	min		: 分
 *	int	sec		: 秒
 *
 * 戻り値
 *	int		: 0	･･･ success
 *			 -1･･･ error code
 *----------------------------------------------------------------------
 */
#include	<limits.h>				/*	"		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	<drive\rtcdrv.h>
#include	"rtcctl.h"

int
rtc_stime(
	int			hour,
	int			min,
	int			sec
	)
{
	int				err = 0,err1;

	ADV_WAI_SEM(err, rtc_nest, ENDLESS,rtc_sem.acadr, VOID_TIME);
	if(err)	return(err|INT_MIN);
	err1 = _rtc_set_time(hour, min, sec);
	ADV_SIG_SEM(err, rtc_nest, rtc_sem.acadr);
	if(err)	return(err|INT_MIN);
	return(err1);
}
