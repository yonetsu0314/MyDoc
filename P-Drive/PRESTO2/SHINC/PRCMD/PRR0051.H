/*======================================================================
 * File name	: prr0051.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	テスト駆動開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0051.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0051.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr051.hv  2.1  93/11/02 15:43:44  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr051.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0051_H)
#define	PRR0051_H


enum prr00510e {			/* テスト駆動			*/
	TDRV_NRML,			/* 通常（連続）			*/
	TDRV_SGNL,			/* 一回動作			*/
};

enum prr00511e {			/* ＲＦ先端シャッタ		*/
	SHT_OFF,			/* 閉じる			*/
	SHT_ON,				/* 開く				*/
};
/*------------------------------------------------------*/
/*	テスト駆動開始データのパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0051t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	cf_ptn;		/* CF pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned long	af_ptn;		/* AF pattern			*/
	unsigned long	rf_ptn;		/* RF pattern			*/
	unsigned long	sht_ptn;	/* RF SHUTTER pattern		*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	wh2_ptn;	/* WH2 pattern(BH side)		*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned short	rs_ptn;		/* RS pattern			*/
	unsigned short	dth_ptn;	/* DTH pattern			*/
	unsigned short	th_ptn;		/* TH pattern			*/
	unsigned long	spa_ptn;	/* spare pattern		*/
	enum prr00510e	drv_cyc;	/* Drive cycle			*/
	enum prr00511e	sht_flg;	/* SHUTTER control flag		*/
	unsigned char	spa_flg;	/* spare cntrol flag		*/
} PRR0051T;

#if	defined(STORAGE)
struct prr0051t	prr0051s;
#else
#if	!defined(NOEXTERN)
extern struct prr0051t	prr0051s;
#endif
#endif

#endif
