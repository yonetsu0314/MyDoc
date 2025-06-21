/*======================================================================
 * File name    : rzi017.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi017.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi017.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd//RCS/rzi017.h 2.7 1994/10/25 15:07:18 hiroe
 *Log: rzi017.h
 * リビジョン 2.7  1994/10/25  15:07:18  hiroe
 * エラーの種類に再チェックエラーを追加する(ERROR_RCH)
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI017_H)
#define	RZI017_H


enum rzi0170e {				/* 異常内容			*/
	ERROR_ZERO,			/* 零点				*/
	ERROR_SPAN,			/* スパン			*/
	ERROR_WH,			/* ＷＨ				*/
	ERROR_PH,			/* ＰＨ				*/
	ERROR_BH,			/* ＢＨ				*/
	ERROR_RS,			/* ＲＳ				*/
	ERROR_DTH,			/* ＤＴＨ			*/
	ERROR_TH,			/* ＴＨ				*/
	ERROR_PW,			/* 単重				*/
	ERROR_OVSCL,			/* オーバースケール		*/
	ERROR_OVWGT,			/* 過量				*/
	ERROR_AFD,			/* AFD				*/
	ERROR_CHT,			/* 上下動シュート		*/
	ERROR_RCH,			/* 再チェックエラー（個数計量）	*/
	ERROR_NON = 15,			/* 拡張側エラー			*/
};

/*------------------------------------------------------*/
/*	異常情報のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzi0170e	error;		/* 異常情報			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned char	rs_ptn;		/* RS pattern			*/
	unsigned char	dth_ptn;	/* DTH pattern			*/
	unsigned char	th_ptn;		/* TH pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
} RZI017T;

#if	defined(STORAGE)
struct rzi017t	rzi017s;
#else
#if	!defined(NOEXTERN)
extern struct rzi017t	rzi017s;
#endif
#endif

#endif
