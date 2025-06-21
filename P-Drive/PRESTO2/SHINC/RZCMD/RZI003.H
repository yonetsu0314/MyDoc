/*======================================================================
 * File name    : rzi003.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi003.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi003.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd//RCS/rzi003.h 2.3 1994/09/28 08:10:06 hiroe
 *Log: rzi003.h
 * リビジョン 2.3  1994/09/28  08:10:06  hiroe
 * サンプル排出用終了フラグを用意する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI003_H)
#define	RZI003_H


enum rzi0030e {				/* 計算結果			*/
	RSLT_MPROPER,			/* 回数計量途中正量		*/
	RSLT_OVER,			/* 過量				*/
	RSLT_PROPER,			/* 正量				*/
	RSLT_UNDER=4,			/* 軽量				*/
	RSLT_SMPL=8,			/* サンプル排出終了フラグ	*/
};

enum rzi0031e {				/* 状態				*/
	HSTS_SELE = 0x0001,		/* 選択				*/
	HSTS_STBL = 0x0002,		/* 安定				*/
	HSTS_EMPTY = 0x0004,		/* 空				*/
	HSTS_AZERO = 0x0008,		/* 自動零点			*/
	HSTS_ERR = 0x0010,		/* エラー			*/
	HSTS_OVSCL = 0x0020,		/* オーバースケール		*/
	HSTS_PFS = 0x0040,		/* プラスフルスケール		*/
	HSTS_MFS = 0x0080,		/* マイナスフルスケール		*/
};

typedef struct rzi0030t {
	enum rzi0030e	flag;		/* 演算結果フラグ		*/
	long		weight;		/* 組み合わせ重量		*/
	short		count;		/* 組み合わせ個数		*/
} RZI0030T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi003t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzi0030t	sect[8];	/* セクション演算結果		*/
	struct rzi0030t	ch;		/* チャネル演算結果		*/
	long		slv_targ_wt;	/* スレーブ機目標重量		*/
	short		slv_targ_cnt;	/* スレーブ機目標個数		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	enum rzi0031e	wh_status[32];	/* WHステータス[nof_wh]		*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	enum rzi0031e	bh_status[32];	/* BHステータス[nof_bh]		*/
} RZI003T;

#if	defined(STORAGE)
struct rzi003t	rzi003s;
#else
#if	!defined(NOEXTERN)
extern struct rzi003t	rzi003s;
#endif
#endif

#endif
