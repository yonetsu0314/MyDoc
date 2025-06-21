/*======================================================================*/
/* File name	: TIME.h												*/
/*----------------------------------------------------------------------*
 *$Header: /RCU32LIB/INCLUDE/lrom/TIME.H 7     02/01/21 15:11 Ohtani $
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1997 ISHIDA CO., LTD.								*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*/
/*		Phone +81 775 53-4141											*/
/*----------------------------------------------------------------------*/

#if	!defined(_TIME_H_)
#define	_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		time_t		ishida_time_t
#define		tm			ishida_tm
#define		localtime	ishida_localtime

#ifndef _TIME_T_DEFINED
typedef long time_t;
#define _TIME_T_DEFINED
#endif
#ifndef _DATE_T_DEFINED
typedef long date_t;
#define _DATE_T_DEFINED
#endif

#ifndef _TM_RCU_DEFINED
struct tm {
	short tm_sec;
	short tm_min;
	short tm_hour;
	short tm_non;
};
#define _TM_RCU_DEFINED
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

struct tm *localtime(const time_t *);
struct dt *localdate(const date_t *);
unsigned char dformat(unsigned char num);
char *asctime(const struct tm *);
char *ctime(const time_t *);
char *ascdate(const struct dt *);
char *cdate(const date_t *);
time_t time(time_t *);
date_t date(date_t *);

int stime(int hour, int minutes, int seconds);
int sdate(int year, int month, int day,int week);
int	country(int num);
void rtc_init(void);

void timer_init(void);
void timer_exit(void);
int timer(int no, int cnt);
void sleep(int time);
int alarm(int time);

#ifdef __cplusplus
}
#endif

#endif	/* _TIME_H_ */
