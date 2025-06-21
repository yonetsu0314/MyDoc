/*======================================================================
 * File name    : rzi031.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	クロスフィーダ制御結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi031.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi031.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi031.hv  1.1  94/02/21 15:54:58  ryoji
 *Log: p:/include/rzcmd/rcs/rzi031.hv
 * Revision 1.1  94/02/21 15:54:58  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI031_H)
#define	RZI031_H

typedef struct rzi0310t {
	unsigned char	amp;		/* フィーダ振幅強度		*/
	unsigned char	time;		/* フィーダ振幅時間		*/
} RZI0310T;

typedef struct rzi0311t {
	unsigned char	nof_sig;	/* 供給状態数			*/
	struct rzi0310t	at[8];		/* rzi0310t paramter[nof_sig]	*/
	unsigned char	sig_stat;	/* 供給状態			*/
	short		df_wt;		/* 合成分散重量			*/
} RZI0311T;

/*------------------------------------------------------*/
/*	ＣＦ制御結果パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi031t {
	unsigned char	seq_no;		/* シーケンス番号		*/
	unsigned char	nof_cf;		/* 該当するCHのCF数		*/
	struct rzi0311t	cf[8];		/* CF paramter[nof_cf]		*/
} RZI031T;

#if	defined(STORAGE)
struct rzi031t	rzi031s;
#else
#if	!defined(NOEXTERN)
extern struct rzi031t	rzi031s;
#endif
#endif

#endif
