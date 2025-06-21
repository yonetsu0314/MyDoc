/*======================================================================
 * File name	: prr1005.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給振り分け装置ゲート位置拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/rcs/prr1005.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr1005.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2006, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR1005_H)
#define	PRR1005_H

enum prr10050e {		/* 振り分け装置のゲート位置	*/
	GATE_FULL,		/* 全開位置			*/
	GATE_HALF,		/* 半開位置			*/
};

/*------------------------------------------------------*/
/*	供給振り分け装置ゲート位置データ		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr1005t {
	unsigned char	nof_ch;			/* number of channel		*/
	enum prr10050e	gate_position[4];	/* 供給振り分け装置ゲート位置	*/
} PRR1005T;

#if	defined(STORAGE)
struct prr1005t	prr1005s;
#else
#if	!defined(NOEXTERN)
extern struct prr1005t	prr1005s;
#endif
#endif

#endif
