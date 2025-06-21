/*======================================================================
 * File name	: prr0057.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0057.h 1.3 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0057.h $
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2004/07/14  09:45
 * スペルミス修正。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/05/08  15:58
 * 新ＡＦＤに関するコマンドを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr057.hv  1.1  93/11/02 17:16:20  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr057.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0057_H)
#define	PRR0057_H

enum prr00570e {			/* AFD制御方式			*/
	AFD_MANUAL,			/* 手動モード			*/
	AFD_MULTI_AUTO,			/* 個別制御自動モード		*/
	AFD_MULTI_TIME,			/* 個別制御自動時間モード	*/
	AFD_MULTI_AMP,			/* 個別制御自動振幅モード	*/
	AFD_SINGLE_AUTO,		/* CCWS方式自動モード		*/
	AFD_SINGLE_TIME,		/* CCWS方式自動時間モード	*/
	AFD_SINGLE_AMP,			/* CCWS方式自動振幅モード	*/
	ADV_AFD_AUTO,			/* 新AFD自動モード		*/
	ADV_AFD_TIME,			/* 新AFD自動時間モード		*/
	ADV_AFD_AMP,			/* 新AFD自動振幅モード		*/
};

enum prr00571e {			/* AFD動作ＳＷ			*/
	DF_CTRL = 0x0001,		/* ＤＦをＡＦＤ制御する		*/
	AF_CTRL = 0x0002,		/* ＡＦをＡＦＤ制御する		*/
};

typedef struct prr00570t {
	enum prr00570e	ctrl_mode;	/* AFD control mode		*/
	enum prr00571e	ctrl_sw;	/* AFD動作ＳＷ			*/
	unsigned char	amp_max;	/* 振幅強度絶対上限		*/
	unsigned char	amp_min;	/* 振幅強度絶対下限		*/
	unsigned char	time_max;	/* 振幅時間絶対上限		*/
	unsigned char	time_min;	/* 振幅時間絶対下限		*/
} PRR00570T;

typedef struct prr00571t {
	unsigned char	amp_max;	/* RF振幅強度絶対上限		*/
	unsigned char	amp_min;	/* RF振幅強度絶対下限		*/
	unsigned char	time_max;	/* RF振幅時間絶対上限		*/
	unsigned char	time_min;	/* RF振幅時間絶対下限		*/
} PRR00571T;

/*------------------------------------------------------*/
/*	ＡＦＤヘッドデータパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0057t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	PRR00570T	sect[8];	/* AFD parameter[nof_sect]	*/
	unsigned char	nof_df;		/* DFの数			*/
	PRR00571T	df[8];		/* DF parameter[nof_df]		*/
	unsigned char	nof_af;		/* AFの数			*/
	PRR00571T	af[32];		/* AF parameter[nof_af]		*/
} PRR0057T;

#if	defined(STORAGE)
struct prr0057t	prr0057s;
#else
#if	!defined(NOEXTERN)
extern struct prr0057t	prr0057s;
#endif
#endif

#endif
