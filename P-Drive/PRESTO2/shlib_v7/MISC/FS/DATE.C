/*======================================================================
 * File name    : date.c
 * Original		: p:/dacs_wrd/rcu/main/rcs/date.c 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		日付／時刻データ・アクセスモジュール定義ファイル
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/fs/RCS/date.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: date.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ファイル・システムとFLASH書込み
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_DATE_C_
#include <string.h>
#include <drive\rtcdrv.h>
#include "utype.h"
#include "date.h"


/* 現在日付時刻管理変数 */
DATE_INFO cur_date;
#ifdef	_DEBUG_ON_PC98_
static DATE_INFO base_date = {95, 12, 5, 2, 18, 30, 0};
#endif

/*****************************************************************************
*
*	関数名: WORD year_adjust(WORD year)
*	機　能: 年ﾃﾞｰﾀの補正処理(西暦上２桁を補正する)。
*
*	引き数:	WORD year  -  年ﾃﾞｰﾀ
*
*	戻り値: WORD  -  補正年ﾃﾞｰﾀ
*
******************************************************************************/
WORD year_adjust(WORD year)
{
	year %= 100;
	if(year >= BASE_YEAR_L) {
		year += BASE_YEAR_19;
	} else {
		year += BASE_YEAR_20;
	}
	return year;
}
/*****************************************************************************
*
*	関数名: void set_date(DATE_INFO *date)
*	機　能: 日付設定処理。
*
*	引き数:	DATE_INFO *date  -  設定する日付ﾃﾞｰﾀを示す
*
*	戻り値: なし
*
******************************************************************************/
void set_date(DATE_INFO *date)
{
#ifndef	_DEBUG_ON_PC98_
	rtc_sdate((date->year % 100), date->month, date->day, date->week);
#else
	base_date.year = date->year;
	base_date.month = date->month;
	base_date.day = date->day;
	base_date.week = date->week;
#endif
}
/*****************************************************************************
*
*	関数名: void get_date(DATE_INFO *date)
*	機　能: 日付ﾃﾞｰﾀの取得。
*
*	引き数:	DATE_INFO *date  -  取得した日付ﾃﾞｰﾀの格納ｴﾘｱを示す
*
*	戻り値: なし
*
******************************************************************************/
void get_date(DATE_INFO *date)
{
#ifndef	_DEBUG_ON_PC98_
	rtc_gdate(&date->year, &date->month, &date->day, &date->week);
#else
	date->year = base_date.year;
	date->month = base_date.month;
	date->day = base_date.day;
	date->week = base_date.week;
#endif
	date->year = (int)year_adjust((WORD)(date->year));
}
/*****************************************************************************
*
*	関数名: void set_time(DATE_INFO *date)
*	機　能: 時刻設定処理。
*
*	引き数:	DATE_INFO *date  -  設定する時刻ﾃﾞｰﾀを示す
*
*	戻り値: なし
*
******************************************************************************/
void set_time(DATE_INFO *date)
{
#ifndef	_DEBUG_ON_PC98_
	rtc_stime(date->hour, date->minute, date->second);
#else
	base_date.hour = date->hour;
	base_date.minute = date->minute;
	base_date.second = date->second;
#endif
}
/*****************************************************************************
*
*	関数名: void get_time(DATE_INFO *date)
*	機　能: 時刻ﾃﾞｰﾀの取得。
*
*	引き数:	DATE_INFO *date  -  取得した時刻ﾃﾞｰﾀの格納ｴﾘｱを示す
*
*	戻り値: なし
*
******************************************************************************/
void get_time(DATE_INFO *date)
{
#ifndef	_DEBUG_ON_PC98_
	rtc_gtime(&date->hour, &date->minute, &date->second);
#else
	date->hour = base_date.hour;
	date->minute = base_date.minute;
	date->second = base_date.second;
#endif
}
/*****************************************************************************
*
*	関数名: void init_date_time(void)
*	機　能: 日付時刻初期設定。
*
*	引き数:	なし
*
*	戻り値: なし
*
******************************************************************************/
void init_date_time(void)
{
	get_date(&cur_date);
	get_time(&cur_date);
}

