/*======================================================================
 * File name    : rzr019.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr019.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr019.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr019.hv  2.1  93/11/02 15:42:44  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr019.hv
 * Revision 2.1  93/11/02 15:42:44  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR019_H)
#define	RZR019_H

enum rzr0190e {				/* バックアップ			*/
	BKUP_NRML,			/* 通常				*/
	BKUP_BKUP,			/* バックアップ			*/
};

enum rzr0191e {				/* ＡＦＤ			*/
	AFD_MANU,			/* 手動				*/
	AFD_AUTO,			/* 自動				*/
	AFD_SEMI,			/* 半自動			*/
};

typedef struct rzr0190t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} RZR0190T;

typedef struct rzr0191t {
	unsigned short	targ;		/* 目標重量			*/
	unsigned char	upper;		/* 上限偏差(%)			*/
	unsigned char	lower;		/* 下限偏差(%)			*/
} RZR0191T;

/*------------------------------------------------------*/
/*	供給量パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr019t {
	unsigned char	ch_id;		/* channel number		*/
	enum rzr0190e	backup;		/* backup			*/
	enum rzr0191e	afd_mode;	/* AFD モード			*/
	unsigned char	min_rf_no;	/* 最小RF番号			*/
	unsigned char	nof_rf;		/* 該当するCHのRF数		*/
	struct rzr0190t	rf[32];		/* RF paramter[nof_rf]		*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するCHのDF数		*/
	struct rzr0190t	df[8];		/* DF paramter[nof_df]		*/
	struct rzr0191t	lev[8];		/* Level paramter[nof_df]	*/
} RZR019T;

#if	defined(STORAGE)
struct rzr019t	rzr019s;
#else
#if	!defined(NOEXTERN)
extern struct rzr019t	rzr019s;
#endif
#endif

#endif
