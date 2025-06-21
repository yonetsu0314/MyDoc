/*======================================================================
 * File name    : rzr067.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	金属検出機テストデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr067.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr067.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzr067.h 1.2 1994/07/12 14:47:11 hiroe
 *Log: rzr067.h
 * リビジョン 1.2  1994/07/12  14:47:11  hiroe
 * READY,GOが、別のファイルにも設定されていたので、HEAD_READY,HEAD_GOに修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR067_H)
#define	RZR067_H

enum rzr0670e {				/* テスト機能			*/
	HEAD_READY,			/* 準備（指定ヘッド停止）	*/
	HEAD_GO,			/* 開始（指定ヘッド優先排出）	*/
};

/*------------------------------------------------------*/
/*	金属検出機テストデータの要求パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr067t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzr0670e	function;	/* テスト機能			*/
} RZR067T;

#if	defined(STORAGE)
struct rzr067t	rzr067s;
#else
extern struct rzr067t	rzr067s;
#if	!defined(NOEXTERN)
#endif
#endif

#endif
