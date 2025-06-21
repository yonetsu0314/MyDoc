/*======================================================================
 * File name    : prr099.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	分散シャッター制御データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0099.h 1.1 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0099.h $
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR0099_H)
#define	PRR0099_H

typedef struct prr00990t {
	unsigned char	on_off;			/* 分散シャッターオン・オフ	*/
	unsigned char	fc_op_cnt;		/* 分散シャッター強制開カウント	*/
	unsigned char	fc_op_time;		/* 分散シャッター強制開時間	*/
        unsigned char   fc_cl_time;             /* 分散シャッター強制閉時間     */
	unsigned char	mode;			/* 分散シャッターモード		*/
	unsigned char	nof_sh;			/* 該当するDFのSH数		*/
	unsigned char	min_sh_no;		/* 最小SH番号			*/
	unsigned char	op_time[32];		/* 分散シャッター開時間		*/
} PRR00990T;

/*------------------------------------------------------*/
/*	分散シャッター制御パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0099t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	struct prr00990t sh[8];		/* 分散シャッター情報		*/
} PRR0099T;

#if	defined(STORAGE)
struct prr0099t	prr0099s;
#else
#if	!defined(NOEXTERN)
extern struct prr0099t	prr0099s;
#endif
#endif

#endif
