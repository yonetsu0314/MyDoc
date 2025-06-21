/*======================================================================
 * File name	: rzi033.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	金属検出機テスト結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0033.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0033.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi033.h 1.1 1994/07/08 13:16:19 hiroe Exp
 *log: rzi033.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0033_H)
#define	PRI0033_H

enum pri00330e {			/* テスト結果			*/
	SUCCESS,			/* 成功				*/
	FAILURE,			/* 失敗				*/
	GIVE_UP,			/* 中止				*/
	SD_GIVE_UP,			/* ＳＤ仕様の為の中止		*/
};

/*------------------------------------------------------*/
/*	金属検出機テスト結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0033t {
	unsigned char	ch_id;		/* channel number		*/
	enum pri00330e	answer;		/* 金属検出機テスト結果		*/
} PRI0033T;

#if	defined(STORAGE)
struct pri0033t	pri0033s;
#else
#if	!defined(NOEXTERN)
extern struct pri0033t	pri0033s;
#endif
#endif

#endif
