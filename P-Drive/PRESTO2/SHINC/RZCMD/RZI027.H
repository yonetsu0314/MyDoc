/*======================================================================
 * File name    : rzi027.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	周辺異常発生データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi027.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi027.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzi027.h 1.4 1995/07/20 15:59:10 hiroe
 *Log: rzi027.h
 * リビジョン 1.4  1995/07/20  15:59:10  hiroe
 * ＡＦＶセル出力異常、ＡＦＶセルオーバーフロー異常を追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI027_H)
#define	RZI027_H


enum rzi0270e {				/* 異常内容			*/
	AFV_ZERO_ERR,			/* AFV零点異常			*/
	AFV_ADJ_ERR,			/* AFV調整異常			*/
	AFV_OUT_ERR,			/* AFVセル出力異常		*/
	AFV_OVF_ERR,			/* AFVセルオーバーフロー異常	*/
	AFV_REV_ERR,			/* AFV補正異常			*/
};

/*------------------------------------------------------*/
/*	周辺異常情報のパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi027t {
	enum rzi0270e	error;		/* 異常情報			*/
	unsigned long	ptn;		/* error head bit pattern	*/
} RZI027T;

#if	defined(STORAGE)
struct rzi027t	rzi027s;
#else
#if	!defined(NOEXTERN)
extern struct rzi027t	rzi027s;
#endif
#endif

#endif
