/*======================================================================
 * File name	: prr0019.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0019.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0019.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr019.hv  2.1  93/11/02 15:42:44  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr019.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0019_H)
#define	PRR0019_H

enum prr00190e {			/* バックアップ			*/
	BKUP_NRML,			/* 通常				*/
	BKUP_BKUP,			/* バックアップ			*/
};

typedef struct prr00190t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} PRR00190T;

/*------------------------------------------------------*/
/*	供給量パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0019t {
	enum prr00190e	backup;		/* backup			*/
	unsigned char	nof_rf;		/* 計量機のRF数			*/
	struct prr00190t	rf[32];	/* RF parameter[nof_rf]		*/
	unsigned char	nof_df;		/* 計量機のDF数			*/
	struct prr00190t	df[8];	/* DF parameter[nof_df]		*/
	unsigned char	nof_af;		/* 計量機のAF数			*/
	struct prr00190t	af[32];	/* AF parameter[nof_af]		*/
} PRR0019T;

#if	defined(STORAGE)
struct prr0019t	prr0019s;
#else
#if	!defined(NOEXTERN)
extern struct prr0019t	prr0019s;
#endif
#endif

#endif
