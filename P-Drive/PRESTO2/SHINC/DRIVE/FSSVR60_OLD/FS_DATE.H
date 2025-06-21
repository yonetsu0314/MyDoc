/*======================================================================
 * File name    : fs_date.h
 * Original		: p:/dacs_wrd/rcu/main/rcs/date.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		日付／時刻データ・アクセスモジュール参照宣言ファイル
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/fs_date.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fs_date.h $
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
#ifndef	_FS_DATE_H_
#define	_FS_DATE_H_

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

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
void fs_get_date(DATE_INFO *date);
void fs_get_time(DATE_INFO *date);

#endif	/* _FS_DATE_H_ */
