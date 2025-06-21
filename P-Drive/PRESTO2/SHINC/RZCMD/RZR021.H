/*======================================================================
 * File name    : rzr021.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr021.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr021.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: RZR021.Hv  2.2  94/02/21 15:50:06  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/RZR021.Hv
 * Revision 2.2  94/02/21 15:50:06  ryoji
 * ダブル機シングルクロスフィーダ対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR021_H)
#define	RZR021_H

enum rzr0210e {				/* ＣＦ制御			*/
	CFCTRL_OFF,			/* しない			*/
	CFCTRL_ON,			/* する				*/
};

typedef struct rzr0210t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} RZR0210T;

typedef struct rzr0211t {
	unsigned char	nof_sig;	/* 供給状態数			*/
	struct rzr0210t	at[8];		/* rzr0210t paramter[nof_sig]	*/
} RZR0211T;

/*------------------------------------------------------*/
/*	ＣＦ振幅パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr021t {
	enum rzr0210e	ctrl;		/* 制御				*/
	unsigned char	nof_cf;		/* 該当するCHのCF数		*/
	struct rzr0211t	cf[8];		/* CF paramter[nof_cf]		*/
} RZR021T;

#if	defined(STORAGE)
struct rzr021t	rzr021s;
#else
#if	!defined(NOEXTERN)
extern struct rzr021t	rzr021s;
#endif
#endif

#endif
