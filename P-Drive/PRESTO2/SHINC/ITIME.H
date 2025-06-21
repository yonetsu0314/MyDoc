/*======================================================================*/
/* File name	: ITIME.h						*/
/*----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/RCS/itime.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: itime.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 実態のない関数dformatのﾌﾟﾛﾄﾀｲﾌﾟを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2003 ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(_ITIME_H_)
#define	_ITIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ITIME_T_DEFINED
typedef long ishida_time_t;
#define _ITIME_T_DEFINED
#endif
#ifndef _DATE_T_DEFINED
typedef long date_t;
#define _DATE_T_DEFINED
#endif

#ifndef _ITM_RCU_DEFINED
struct ishida_tm {
	short tm_sec;
	short tm_min;
	short tm_hour;
	short tm_non;
};
#define _ITM_RCU_DEFINED
#endif
#ifndef _DT_RCU_DEFINED
struct dt {
	short tm_wday;
	short tm_mday;
	short tm_mon;
	short tm_year;
};
#define _DT_RCU_DEFINED
#endif

struct ishida_tm *ishida_localtime(const ishida_time_t *);
struct dt *localdate(const date_t *);
//unsigned char dformat(unsigned char num);
char *asctime(const struct ishida_tm *);
char *ctime(const ishida_time_t *);
char *ascdate(const struct dt *);
char *cdate(const date_t *);
ishida_time_t time(ishida_time_t *);
date_t date(date_t *);

int stime(int hour, int minutes, int seconds);
int sdate(int year, int month, int day,int week);
int	country(int num);
void rtc_init(void);

void timer_init(void);
void timer_exit(void);
int timer(int no, int cnt);
//void sleep(int time);
int alarm(int time);

#ifdef __cplusplus
}
#endif

#endif	/* _ITIME_H_ */
