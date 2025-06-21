/*======================================================================
 * File name	: pri0031.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0031.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0031.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzi031.hv  1.1  94/02/21 15:54:58  ryoji  Exp
 *log: p:/include/rzcmd/rcs/rzi031.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0031_H)
#define	PRI0031_H

typedef struct pri00310t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} PRI00310T;

typedef struct pri00311t {
	unsigned char	nof_sig;	/* 供給状態数			*/
	struct pri00310t	at[8];	/* parameter[nof_sig]		*/
	unsigned char	sig_stat;	/* 供給状態			*/
	short		df_wt;		/* 合成分散重量			*/
} PRI00311T;

/*------------------------------------------------------*/
/*	ＣＦ制御結果パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0031t {
	unsigned char	seq_no;		/* シーケンス番号		*/
	unsigned char	nof_cf;		/* 該当するCHのCF数		*/
	struct pri00311t	cf[8];	/* CF parameter[nof_cf]		*/
} PRI0031T;

#if	defined(STORAGE)
struct pri0031t	pri0031s;
#else
#if	!defined(NOEXTERN)
extern struct pri0031t	pri0031s;
#endif
#endif

#endif
