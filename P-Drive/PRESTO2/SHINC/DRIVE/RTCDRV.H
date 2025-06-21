/*----------------------------------------------------------------------*
 * File name	: rtcdrv.h	/ rtc driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/rtcdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rtcdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(RTCDRV_H)
#define	RTCDRV_H

int		rtc_sys_init(void);
int		rtc_sdate(int year, int month, int day, int week);
int		rtc_gdate(int *year, int *month, int *day, int *week);
int		rtc_stime(int hour, int min, int sec);
int		rtc_gtime(int *hour, int *min, int *sec);

#endif	/* RTCDRV_H */

