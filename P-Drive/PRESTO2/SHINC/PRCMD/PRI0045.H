/*======================================================================
 * File name    : pri0045.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動用状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0045.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0045.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi045.h 1.5 1970/01/01 00:00:00 hiroe
 *Log: rzi045.h
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRI0045_H)
#define	PRI0045_H

enum pri00450e {					/* 状態内容							*/
	NHIC_COMERR,					/* ＮＨＩＣ間通信エラー				*/
	NHIC_ERROR,						/* ＮＨＩＣ内部エラー				*/
	ICU_COMERR,						/* NHIC-ICU間通信エラー				*/
	NETWORK_INF,					/* ネットワーク情報					*/
	NHICACT_INF,					/* ＮＨＩＣ動作情報					*/
	DEBUG_INF,						/* デバッグ情報						*/
};

enum pri00451e {					/* ＲＣＵ動作情報					*/
	ERR,							/* エラー停止						*/
	WARNING,						/* 表示のみ(ブザーオン)				*/
	DISPLAY,						/* 表示のみ（ブザーオフ）			*/
	PRINT,							/* 印刷								*/
	ERR_PRN,						/* エラー停止 ＆ 印刷				*/
	WARN_PRN,						/* 表示のみ(ブザーオン) & 印刷		*/
	DISP_PRN,						/* 表示のみ(ブザーオフ) & 印刷		*/
};

typedef union pri00450t {
	char			c_data[16];
	short			i_data[8];
	long			l_data[4];
} PRI00450T;

typedef struct pri00451t {
	unsigned char	data_cnt;		/* データバイト数	*/
	PRI00450T		dt;				/* データ			*/
} PRI00451T;

typedef struct pri00452t {
	unsigned char	disp_cnt;		/* データバイト数	*/
	char			disp_str[128];	/* 表示データ		*/
} PRI00452T;

typedef struct pri00453t {
	unsigned char	prn_cnt;		/* データバイト数	*/
	char			prn_str[32];	/* 印字データ		*/
} PRI00453T;

/*------------------------------------------------------*/
/*	マルチ連動用状態データのパラメータパケット			*/
/*														*/
/*------------------------------------------------------*/

typedef struct pri0045t {
	unsigned char	ch_id;			/* channel number		*/
	enum pri00450e	cond_inf;		/* 状態内容				*/
	enum pri00451e	act_inf;		/* ＲＣＵ動作情報		*/
	unsigned long	node_ptn;		/* error node pattern	*/
	short			error_no;		/* エラー情報			*/
	PRI00451T		ex_d;			/* 汎用データ			*/
	PRI00452T		ds_d;			/* 表示データ			*/
	PRI00453T		pr_d;			/* 印字データ			*/
} PRI0045T;

#if	defined(STORAGE)
struct pri0045t	pri0045s;
#else
#if	!defined(NOEXTERN)
extern struct pri0045t	pri0045s;
#endif
#endif

#endif
