/*======================================================================
 * File name    : rzr099.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	分散シャッター制御データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr099.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr099.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr099.h 1.2 1998/07/28 13:13:24 itami
 *Log: rzr099.h
 * リビジョン 1.2  1998/07/28  13:13:24  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR099_H)
#define	RZR099_H

typedef struct rzr0990t {
	unsigned char	on_off;			/* 分散シャッターオン・オフ	*/
	unsigned char	fc_op_cnt;		/* 分散シャッター強制開カウント	*/
	unsigned char	fc_op_time;		/* 分散シャッター強制開時間	*/
        unsigned char   fc_cl_time;             /* 分散シャッター強制閉時間     */
	unsigned char	mode;			/* 分散シャッターモード		*/
	unsigned char	nof_sh;			/* 該当するDFのSH数		*/
	unsigned char	min_sh_no;		/* 最小SH番号			*/
	unsigned char	op_time[32];		/* 分散シャッター開時間		*/
} RZR0990T;

/*------------------------------------------------------*/
/*	分散シャッター制御パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr099t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	struct rzr0990t	sh[8];		/* 分散シャッター情報		*/
} RZR099T;

#if	defined(STORAGE)
struct rzr099t	rzr099s;
#else
#if	!defined(NOEXTERN)
extern struct rzr099t	rzr099s;
#endif
#endif

#endif
