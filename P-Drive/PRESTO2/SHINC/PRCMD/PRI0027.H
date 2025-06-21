/*======================================================================
 * File name	: pri0027.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	周辺異常発生データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0027.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0027.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzi027.h 1.4 1995/07/20 15:59:10 hiroe Exp
 *log: rzi027.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0027_H)
#define	PRI0027_H


enum pri00270e {			/* 異常内容			*/
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

typedef struct pri0027t {
	enum pri00270e	error;		/* 異常情報			*/
	unsigned long	ptn;		/* error head bit pattern	*/
} PRI0027T;

#if	defined(STORAGE)
struct pri0027t	pri0027s;
#else
#if	!defined(NOEXTERN)
extern struct pri0027t	pri0027s;
#endif
#endif

#endif
