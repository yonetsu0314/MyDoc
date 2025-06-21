/*======================================================================
 * File name    : fs_date.c
 * Original		: p:/dacs_wrd/rcu/main/rcs/date.c 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		日付／時刻データ・アクセスモジュール定義ファイル
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility2/rcs/fs_date.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fs_date.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_FS_DATE_C_
#include "fs_date.h"
#include "drive\rtcdrv.h"

/*****************************************************************************
*
*	関数名: static unsigned short fs_year_adjust(unsigned short year)
*	機　能: 年ﾃﾞｰﾀの補正処理(西暦上２桁を補正する)。
*
*	引き数:	unsigned short year  -  年ﾃﾞｰﾀ
*
*	戻り値: unsigned short  -  補正年ﾃﾞｰﾀ
*
******************************************************************************/
static unsigned short fs_year_adjust(unsigned short year)
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
*	関数名: void fs_get_date(DATE_INFO *date)
*	機　能: 日付ﾃﾞｰﾀの取得。
*
*	引き数:	DATE_INFO *date  -  取得した日付ﾃﾞｰﾀの格納ｴﾘｱを示す
*
*	戻り値: なし
*
******************************************************************************/
void fs_get_date(DATE_INFO *date)
{
	rtc_gdate(&date->year, &date->month, &date->day, &date->week);
	date->year = (int)fs_year_adjust((unsigned short)(date->year));
}
/*****************************************************************************
*
*	関数名: void fs_get_time(DATE_INFO *date)
*	機　能: 時刻ﾃﾞｰﾀの取得。
*
*	引き数:	DATE_INFO *date  -  取得した時刻ﾃﾞｰﾀの格納ｴﾘｱを示す
*
*	戻り値: なし
*
******************************************************************************/
void fs_get_time(DATE_INFO *date)
{
	rtc_gtime(&date->hour, &date->minute, &date->second);
}

