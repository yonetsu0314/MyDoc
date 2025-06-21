/*======================================================================
 * File name	: prr0067.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	金属検出機テストデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0067.h 1.1 1970/01/01 00:00:00 hiroe Exp $
 *$Log: prr0067.h $
 * リビジョン 1.1  1970/01/01  00:00:00  hiroe
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto/shinc/rzcmd/RCS/rzr067.h 1.1 1970/01/01 00:00:00 chimura Exp 
 *Log: rzr067.h 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0067_H)
#define	PRR0067_H

enum prr00670e {				/* テスト機能			*/
	HEAD_READY,			/* 準備（指定ヘッド停止）	*/
	HEAD_GO,			/* 開始（指定ヘッド優先排出）	*/
};

/*------------------------------------------------------*/
/*	金属検出機テストデータの要求パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0067t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum prr00670e	function;	/* テスト機能			*/
} PRR0067T;

#if	defined(STORAGE)
struct prr0067t	prr0067s;
#else
extern struct prr0067t	prr0067s;
#if	!defined(NOEXTERN)
#endif
#endif

#endif
