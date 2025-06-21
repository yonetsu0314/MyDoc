/*----------------------------------------------------------------------
 * File name	: rtcsdt.c	/ rtc set date
 * Original	: p:/lib/drive/rtc/rcs/rtcsdt.c 1.2
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcsdt.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rtcsdt.c $
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
 *	RTCに日付をセットする
 *----------------------------------------------------------------------
 *  引き数
 *	int	year	: 年
 *	int	month	: 月
 *	int	day		: 日
 *	int	week	: 曜日
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
rtc_sdate(
	int			year,
	int			month,
	int			day,
	int			week
	)
{
	int				err = 0,err1;

	ADV_WAI_SEM(err, rtc_nest, ENDLESS,rtc_sem.acadr, VOID_TIME);
	if(err)	return(err|INT_MIN);
	err1 = _rtc_set_date(year, month, day, week);
	ADV_SIG_SEM(err, rtc_nest, rtc_sem.acadr);
	if(err)	return(err|INT_MIN);
	return(err1);
}
