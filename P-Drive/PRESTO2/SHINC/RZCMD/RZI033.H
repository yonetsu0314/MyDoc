/*======================================================================
 * File name    : rzi033.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	金属検出機テスト結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi033.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi033.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi033.h 1.1 1994/07/08 13:16:19 hiroe
 *Log: rzi033.h
 * リビジョン 1.1  1994/07/08  13:16:19  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI033_H)
#define	RZI033_H

enum rzr0330e {				/* テスト結果			*/
	SUCCESS,			/* 成功				*/
	FAILURE,			/* 失敗				*/
	GIVE_UP,			/* 中止				*/
	SD_GIVE_UP,			/* ＳＤ仕様の為の中止		*/
};

/*------------------------------------------------------*/
/*	金属検出機テスト結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi033t {
	unsigned char	ch_id;		/* channel number		*/
	enum rzr0330e	answer;		/* 金属検出機テスト結果		*/
} RZI033T;

#if	defined(STORAGE)
struct rzi033t	rzi033s;
#else
#if	!defined(NOEXTERN)
extern struct rzi033t	rzi033s;
#endif
#endif

#endif
