/*======================================================================
 * File name    : date.h
 * Original		: p:/dacs_wrd/rcu/main/rcs/date.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		日付／時刻データ・アクセスモジュール参照宣言ファイル
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/fs/RCS/date.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: date.h $
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
#ifndef	_DATE_H_
#define	_DATE_H_
#include "utype.h"

/*
// 	日付時刻管理用定数定義
*/
#define	BASE_YEAR_L		(92)	/* 基準年(西暦下２桁) */
#define	BASE_YEAR_19	(1900)	/* 1900年ﾍﾞｰｽ値 */
#define	BASE_YEAR_20	(2000)	/* 2000年ﾍﾞｰｽ値 */

/*
// 	日付時刻管理変数構造体 saka:WORDをintに変更
*/
typedef struct {
	int 	year;		/* 年 */
	int 	month;      /* 月 */
	int 	day;        /* 日 */
	int 	week;       /* 曜日 */
	int 	hour;       /* 時 */
	int 	minute;     /* 分 */
	int 	second;     /* 秒 */
}
	DATE_INFO;

#ifndef	_DATE_C_
/* 現在日付時刻管理変数 */
extern	DATE_INFO cur_date;

#endif	/* _DATE_C_ */
/*
// ﾏｸﾛ関数
*/
#define	date_renew() get_date(&cur_date)
#define	time_renew() get_time(&cur_date)

#define	MINUTE_OF_DAY	(24 * 60)
#define	time_to_minute(hour, minute)\
(((hour) * 60 + (minute)) % MINUTE_OF_DAY)
/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
WORD year_adjust(WORD year);
void set_date(DATE_INFO *date);
void get_date(DATE_INFO *date);
void set_time(DATE_INFO *date);
void get_time(DATE_INFO *date);
void init_date_time(void);

void date_fmt_str(DATE_INFO *date, BYTE *str, int month, int zf);
#define	DATE_STR_LENGTH	(11)/* 日付表示文字列の長さ */
#define	DATE_STR_SIZE	(DATE_STR_LENGTH + 1)

void time_fmt_str(DATE_INFO *date, BYTE *str, int zf);
#define	TIME_STR_LENGTH	(5)/* 時刻表示文字列の長さ */
#define	TIME_STR_SIZE	(TIME_STR_LENGTH + 1)

void month_disp_str(int month, BYTE *str);
#define	MONTH_NAME_LENGTH	(3)/* 月名表示文字列の長さ */
#define	MONTH_NAME_SIZE	(MONTH_NAME_LENGTH + 1)

void date_time_fmt_str(DATE_INFO *date, BYTE *str, int zf);
#define	DATE_TIME_STR_LENGTH	(12)/* 日付時刻表示文字列の長さ */
#define	DATE_TIME_STR_SIZE	(DATE_TIME_STR_LENGTH + 1)

#endif	/* _DATE_H_ */
