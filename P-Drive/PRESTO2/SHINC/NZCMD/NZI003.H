/*======================================================================
 * File name    : nzi003.h
 * Original	: rzi003.h
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi003.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi003.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi003.h 1.1 1999/05/27 16:35:00 kawamura
 *Log: nzi003.h
 * リビジョン 1.1  1999/05/27  16:35:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI003_H)
#define	NZI003_H


enum nzi0030e {				/* 計算結果			*/
	RSLT_MPROPER,			/* 回数計量途中正量		*/
	RSLT_OVER,			/* 過量				*/
	RSLT_PROPER,			/* 正量				*/
	RSLT_UNDER=4,			/* 軽量				*/
	RSLT_SMPL=8,			/* サンプル排出終了フラグ	*/
};

enum nzi0031e {				/* 状態				*/
	HSTS_SELE = 0x0001,		/* 選択				*/
	HSTS_STBL = 0x0002,		/* 安定				*/
	HSTS_EMPTY = 0x0004,		/* 空				*/
	HSTS_AZERO = 0x0008,		/* 自動零点			*/
	HSTS_ERR = 0x0010,		/* エラー			*/
	HSTS_OVSCL = 0x0020,		/* オーバースケール		*/
	HSTS_PFS = 0x0040,		/* プラスフルスケール		*/
	HSTS_MFS = 0x0080,		/* マイナスフルスケール		*/
	HSTS_4TH = 0x0100,		/* ４段目データ採用		*/
};

typedef struct nzi0030t {
	enum nzi0030e	flag;		/* 演算結果フラグ		*/
	long		weight;		/* 組み合わせ重量		*/
	short		count;		/* 組み合わせ個数		*/
} NZI0030T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi003t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct nzi0030t	sect[8];	/* セクション演算結果		*/
	struct nzi0030t	ch;		/* チャネル演算結果		*/
	long		slv_targ_wt;	/* スレーブ機目標重量		*/
	short		slv_targ_cnt;	/* スレーブ機目標個数		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	enum nzi0031e	wh_status[32];	/* WHステータス[nof_wh]		*/
	unsigned char	nof_bh;		/* 該当するchのBH数		*/
	enum nzi0031e	bh_status[32];	/* BHステータス[nof_bh]		*/
} NZI003T;

#if	defined(STORAGE)
struct nzi003t	nzi003s;
#else
#if	!defined(NOEXTERN)
extern struct nzi003t	nzi003s;
#endif
#endif

#endif
