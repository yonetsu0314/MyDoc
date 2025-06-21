/*======================================================================
 * File name	: prr0021.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0021.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0021.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: RZR021.Hv  2.2  94/02/21 15:50:06  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/RZR021.Hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0021_H)
#define	PRR0021_H

enum prr00210e {			/* ＣＦ制御			*/
	CFCTRL_OFF,			/* しない			*/
	CFCTRL_ON,			/* する				*/
};

typedef struct prr00210t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} PRR00210T;

typedef struct prr00211t {
	unsigned char		nof_sig;	/* 供給状態数			*/
	struct prr00210t	at[8];		/* prr00210t parameter[nof_sig]	*/
} PRR00211T;

/*------------------------------------------------------*/
/*	ＣＦ振幅パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0021t {
	enum prr00210e	ctrl;		/* 制御				*/
	unsigned char	nof_cf;		/* 該当するCHのCF数		*/
	struct prr00211t	cf[8];	/* CF parameter[nof_cf]		*/
} PRR0021T;

#if	defined(STORAGE)
struct prr0021t	prr0021s;
#else
#if	!defined(NOEXTERN)
extern struct prr0021t	prr0021s;
#endif
#endif

#endif
