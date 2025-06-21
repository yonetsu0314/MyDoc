/*======================================================================
 * File name    : rzi045.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動用状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi045.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi045.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi045.h 1.5 1970/01/01 00:00:00 hiroe
 *Log: rzi045.h
 * リビジョン 1.5  1970/01/01  00:00:00  hiroe
 * 2000/09/11 21:12
 * ERROR -> ERR
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI045_H)
#define	RZI045_H

enum rzi0450e {				/* 状態内容			*/
	NHIC_COMERR,			/* ＮＨＩＣ間通信エラー		*/
	NHIC_ERROR,			/* ＮＨＩＣ内部エラー		*/
	ICU_COMERR,			/* NHIC-ICU間通信エラー		*/
	NETWORK_INF,			/* ネットワーク情報		*/
	NHICACT_INF,			/* ＮＨＩＣ動作情報		*/
	DEBUG_INF,			/* デバッグ情報			*/
};

enum rzi0451e {				/* ＲＣＵ動作情報		*/
	ERR,				/* エラー停止			*/
	WARNING,			/* 表示のみ(ブザーオン)		*/
	DISPLAY,			/* 表示のみ（ブザーオフ）	*/
	PRINT,				/* 印刷				*/
	ERR_PRN,			/* エラー停止 ＆ 印刷		*/
	WARN_PRN,			/* 表示のみ(ブザーオン) & 印刷	*/
	DISP_PRN,			/* 表示のみ(ブザーオフ) & 印刷	*/
};

typedef union rzi0450t {
	char	c_data[16];
	short	i_data[8];
	long	l_data[4];
} RZI0450T;

typedef struct rzi0451t {
	unsigned char	data_cnt;	/* データバイト数		*/
	RZI0450T	dt;		/* データ			*/
} RZI0451T;

typedef struct rzi0452t {
	unsigned char	disp_cnt;	/* データバイト数		*/
	char		disp_str[128];	/* 表示データ			*/
} RZI0452T;

typedef struct rzi0453t {
	unsigned char	prn_cnt;	/* データバイト数		*/
	char		prn_str[32];	/* 印字データ			*/
} RZI0453T;

/*------------------------------------------------------*/
/*	マルチ連動用状態データのパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi045t {
	unsigned char	ch_id;		/* channel number		*/
	enum rzi0450e	cond_inf;	/* 状態内容			*/
	enum rzi0451e	act_inf;	/* ＲＣＵ動作情報		*/
	unsigned long	node_ptn;	/* error node pattern		*/
	short		error_no;	/* エラー情報			*/
	RZI0451T	ex_d;		/* 汎用データ			*/
	RZI0452T	ds_d;		/* 表示データ			*/
        RZI0453T        pr_d;           /* 印字データ                   */
} RZI045T;

#if	defined(STORAGE)
struct rzi045t	rzi045s;
#else
#if	!defined(NOEXTERN)
extern struct rzi045t	rzi045s;
#endif
#endif

#endif
