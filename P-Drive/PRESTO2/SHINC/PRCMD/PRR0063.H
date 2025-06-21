/*======================================================================
 * File name	: prr0063.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	付加装置拡張パラメータデータ構造
 *	上下動シュート
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0063.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0063.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr063.hv  1.1  94/06/03 10:59:52  fukutome  Exp
 *log: p:/include/rzcmd/rcs/rzr063.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0063_H)
#define	PRR0063_H

typedef struct prr0063t {
	unsigned char	dev_no;		/* 装置番号			*/
	unsigned char	nof_pos;	/* ポジション数			*/
	unsigned char	err_detect_interval;/* エラー検出時間（10ms)	*/
	unsigned char	position1;	/* 設定位置１			*/
	unsigned char	position2;	/* 設定位置２			*/
} PRR0063T;

#if	defined(STORAGE)
struct prr0063t	prr0063s;
#else
#if	!defined(NOEXTERN)
extern struct prr0063t	prr0063s;
#endif
#endif

#endif
