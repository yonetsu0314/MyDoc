/*----------------------------------------------------------------------*
 * File name	: rtc.h	/ RTC bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/rtc.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rtc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/rtc.h 1.2 1995/12/04 13:44:43 ryoji
 *Log: rtc.h
 * リビジョン 1.2  1995/12/04  13:44:43  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(RTC_H)
#define	RTC_H

int	_rtc_initialize(void);
int	_rtc_set_date(int year, int month, int day, int week);
int	_rtc_get_date(int *year, int *month, int *day, int *week);
int	_rtc_set_time(int hour, int min, int sec);
int	_rtc_get_time(int *hour, int *min, int *sec);

#endif
