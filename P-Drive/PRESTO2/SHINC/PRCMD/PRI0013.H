/*======================================================================
 * File name	: pri0013.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0013.h 1.2 1970/01/01 00:00:00 hiroe Exp $
 *$Log: pri0013.h $
 * リビジョン 1.2  1970/01/01  00:00:00  hiroe
 * 2003/12/14
 * ホッパ全開中を追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi013a.h 1.2 1995/11/16 16:14:02 hiroe Exp
 *log: rzi013a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0013_H)
#define	PRI0013_H


enum pri00130e {			/* 状態				*/
	COND_IDLE,			/* 待機中			*/
	COND_PROD,			/* 運転				*/
	COND_ZERO,			/* 零点調整			*/
	COND_SPAN,			/* スパン			*/
	COND_DRAIN,			/* 全排出			*/
	COND_TDRV,			/* テスト駆動			*/
	COND_ERSTOP,			/* エラー停止			*/
	COND_ERREJ,			/* エラー解除			*/
	COND_LOWPRO,			/* 供給不足停止			*/
	COND_HPROPN,			/* ホッパ全開中			*/
	COND_AFV,			/* ＡＦＶ調整中			*/
};

enum pri00131e {			/* 電源状態			*/
	PWER_OFF,			/* オフ				*/
	PWER_ON,			/* オン				*/
};

enum pri00132e {			/* 供給状態			*/
	INFD_OK,			/* 正常				*/
	INFD_NG,			/* 不足				*/
};

/*------------------------------------------------------*/
/*	本体の状態のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri00130t {
	enum pri00130e	condition;	/* 状態				*/
	enum pri00131e	power_stat;	/* 駆動系電源状態		*/
	enum pri00132e	infeed_stat;	/* 供給状態			*/
} PRI00130T;

typedef struct pri0013t {
	unsigned char	nof_ch;		/* number of channel		*/
	struct pri00130t	ch[4];	/* cond parameter of channel	*/
	unsigned char	df_infeed_stat;	/* 分散フィーダ供給状態		*/
} PRI0013T;

#if	defined(STORAGE)
struct pri0013t	pri0013s;
#else
#if	!defined(NOEXTERN)
extern struct pri0013t	pri0013s;
#endif
#endif

#endif
