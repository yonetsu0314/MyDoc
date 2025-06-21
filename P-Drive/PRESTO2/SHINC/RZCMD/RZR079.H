/*======================================================================
 * File name    : rzr079.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダデータ構造２
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr079.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr079.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr079.h 1.3 1996/04/11 16:55:29 hiroki
 *Log: rzr079.h
 * リビジョン 1.3  1996/04/11  16:55:29  hiroki
 * CFのマックスを１６にする。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR079_H)
#define	RZR079_H

enum rzr0790e {				/* ＣＦ制御			*/
	CFCTRL_OFF,			/* しない			*/
	CFCTRL_ON,			/* する				*/
};

typedef struct rzr0790t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} RZR0790T;

typedef struct rzr0791t {
	unsigned char	nof_sig;	/* 供給状態数			*/
	struct rzr0790t	at[8];		/* rzr0210t paramter[nof_sig]	*/
} RZR0791T;

/*------------------------------------------------------*/
/*	ＣＦ振幅パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr079t {
	enum rzr0790e	ctrl;		/* 制御				*/
	unsigned char	nof_cf;		/* 該当するCHのCF数		*/
	struct rzr0791t	cf[16];		/* CF paramter[nof_cf]		*/
} RZR079T;

#if	defined(STORAGE)
struct rzr079t	rzr079s;
#else
#if	!defined(NOEXTERN)
extern struct rzr079t	rzr079s;
#endif
#endif

#endif
