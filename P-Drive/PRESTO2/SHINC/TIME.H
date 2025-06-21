/*----------------------------------------------------------------------*
 * File name	: time.h						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/RCS/time.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: time.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


typedef long time_t;
typedef long date_t;

struct tm {
	short tm_sec;
	short tm_min;
	short tm_hour;
	short tm_non;
};
struct dt {
	short tm_wday;
	short tm_mday;
	short tm_mon;
	short tm_year;
};

struct tm *localtime(time_t *clock);
struct dt *localdate(date_t *clock);
char *asctime(struct tm *tm), *ctime(time_t *clock);
char *ascdate(struct dt *tm), *cdate(date_t *clock);

/*
struct tm *localtime();
struct dt *localdate();
char *asctime(), *ctime();
char *ascdate(), *cdate();
*/
time_t time();
date_t date();
