/*======================================================================
 * File name    : rzi013a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi013a.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi013a.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi013a.h 1.2 1995/11/16 16:14:02 hiroe
 *Log: rzi013a.h
 * リビジョン 1.2  1995/11/16  16:14:02  hiroe
 * 誤記修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI013_H)
#define	RZI013_H


enum rzi0130e {				/* 状態				*/
	COND_IDLE,			/* 待機中			*/
	COND_PROD,			/* 運転				*/
	COND_ZERO,			/* 零点調整			*/
	COND_SPAN,			/* スパン			*/
	COND_DRAIN,			/* 全排出			*/
	COND_TDRV,			/* テスト駆動			*/
	COND_ERSTOP,			/* エラー停止			*/
	COND_ERREJ,			/* エラー解除			*/
	COND_LOWPRO,			/* 供給不足停止			*/
};

enum rzi0131e {				/* 電源状態			*/
	PWER_OFF,			/* オフ				*/
	PWER_ON,			/* オン				*/
};

enum rzi0132e {				/* 供給状態			*/
	INFD_OK,			/* 正常				*/
	INFD_NG,			/* 不足				*/
};

/*------------------------------------------------------*/
/*	本体の状態のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi0130t {
	enum rzi0130e	condition;	/* 状態				*/
	enum rzi0131e	power_stat;	/* 駆動系電源状態		*/
	enum rzi0132e	infeed_stat;	/* 供給状態			*/
} RZI0130T;

typedef struct rzi013t {
	unsigned char	nof_ch;		/* number of channel		*/
	struct rzi0130t	ch[2];		/* cond parameter of channel	*/
	unsigned char	df_infeed_stat;	/* 分散フィーダ供給状態		*/
} RZI013T;

#if	defined(STORAGE)
struct rzi013t	rzi013s;
#else
#if	!defined(NOEXTERN)
extern struct rzi013t	rzi013s;
#endif
#endif

#define		rz_scan_condition	rz_scan_condition_v3
#define		rz_print_condition	rz_print_condition_v3

#endif
