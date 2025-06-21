/*======================================================================
 * File name	: prr0035.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	スパン調整データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0035.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0035.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr035.hv  2.1  93/11/02 15:42:16  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr035.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0035_H)
#define	PRR0035_H


/*------------------------------------------------------*/
/*	スパン校正パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0035t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	adj_head_ptn;	/* 調整ヘッドビットパターン	*/
	unsigned long	date;		/* 調整日付			*/
	unsigned long	time;		/* 調整時間			*/
} PRR0035T;

#if	defined(STORAGE)
struct prr0035t	prr0035s;
#else
#if	!defined(NOEXTERN)
extern struct prr0035t	prr0035s;
#endif
#endif

#endif
