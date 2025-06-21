/*======================================================================
 * File name	: prr0041.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	全排出開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0041.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0041.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr041.hv  2.1  93/11/02 15:43:40  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr041.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0041_H)
#define	PRR0041_H


/*------------------------------------------------------*/
/*	全排出開始パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0041t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned long	head_ptn;	/* 排出ヘッドビットパターン	*/
} PRR0041T;

#if	defined(STORAGE)
struct prr0041t	prr0041s;
#else
#if	!defined(NOEXTERN)
extern struct prr0041t	prr0041s;
#endif
#endif

#endif
