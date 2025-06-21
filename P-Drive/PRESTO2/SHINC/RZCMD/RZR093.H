/*======================================================================
 * File name    : rzr093.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動パラメータ２構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr093.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr093.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr093.h 1.2 1997/12/08 17:31:50 itami
 *Log: rzr093.h
 * リビジョン 1.2  1997/12/08  17:31:50  itami
 * データの型を変更する
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR093_H)
#define	RZR093_H


/*------------------------------------------------------*/
/*	マルチ連動パラメータ２構造			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr093t {
	unsigned char	ch_id;			/* channel number		*/
	unsigned char	multi_onoff;		/* マルチ連動 ＯＮ／ＯＦＦ設定	*/
	unsigned char	multi_enter;		/* マルチ連動 参加／不参加	*/
	unsigned char	combi_delay;		/* 計量遅れ時間			*/
	unsigned char	consec_mode;		/* 順次補正モード		*/
	unsigned char	mlt_dump_mode;		/* 回数計量排出モード		*/
	unsigned char	interface;		/* 包装機連動方式		*/
	unsigned char	speed;			/* 自走時間			*/
	unsigned char	conv_delay;		/* ｺﾝﾍﾞｱ起動信号遅れ時間	*/
	unsigned char	conv_on_time;		/* ｺﾝﾍﾞｱ起動信号オン時間	*/
	unsigned char	ds_delay;		/* 排出完了信号遅れ時間		*/
	unsigned char	ds_on_time;		/* 排出完了信号オン時間		*/
	unsigned char	ds_sht_cnt;		/* 排出完了信号シフト数		*/
	unsigned char	mlt_dump_conf;		/* 回数計量排出完了信号出力ﾓｰﾄﾞ	*/
	unsigned char	p_ds_delay;		/* 正量排出完了信号遅れ時間	*/
	unsigned char	p_ds_on_time;		/* 正量排出完了信号オン時間	*/
	unsigned char	p_ds_sht_cnt;		/* 正量排出完了信号シフト数	*/
} RZR093T;

#if	defined(STORAGE)
struct rzr093t	rzr093s;
#else
#if	!defined(NOEXTERN)
extern struct rzr093t	rzr093s;
#endif
#endif

#endif
