/*======================================================================
 * File name	: pri0003.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0003.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0003.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi003.h 1.1 1999/05/27 16:35:00 kawamura Exp
 *log: nzi003.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0003_H)
#define	PRI0003_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

enum pri00030e {			/* 計算結果			*/
	RSLT_MPROPER,			/* 回数計量途中正量		*/
	RSLT_OVER = 0x0001,		/* 過量				*/
	RSLT_PROPER = 0x0002,		/* 正量				*/
	RSLT_UNDER = 0x0004,		/* 軽量				*/
	RSLT_SMPL = 0x0008,		/* サンプル排出			*/
	RSLT_DUMP = 0x0010,		/* 排出有り			*/
	RSLT_RECK_ERR = 0x0020,		/* 再チェックエラー		*/
	RSLT_OS = 0x0040,		/* オーバースケール排出		*/
};

typedef struct pri00030t {
	enum pri00030e	flag;		/* 演算結果フラグ		*/
	long		weight;		/* 組み合わせ重量		*/
	short		count;		/* 組み合わせ個数		*/
} PRI00030T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0003t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct pri00030t	sect[8];/* セクション演算結果		*/
	struct pri00030t	ch;	/* チャネル演算結果		*/
	unsigned char	multi_dump_seq_no;/* 回数計量排出シーケンス番号	*/
	long		slv_targ_wt;	/* スレーブ機目標重量		*/
	short		slv_targ_cnt;	/* スレーブ機目標個数		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	enum prprm003e	wh_status[32];	/* WHステータス[nof_wh]		*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	enum prprm003e	bh_status[32];	/* BHステータス[nof_bh]		*/
} PRI0003T;

#if	defined(STORAGE)
struct pri0003t	pri0003s;
#else
#if	!defined(NOEXTERN)
extern struct pri0003t	pri0003s;
#endif
#endif

#endif
