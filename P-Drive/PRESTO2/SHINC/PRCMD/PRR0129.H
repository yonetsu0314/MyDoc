/*======================================================================
 * File name	: prr0129.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	運転中にホッパを取り外しての零点データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0129.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0129.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0129_H)
#define	PRR0129_H


/*--------------------------------------------------------------*/
/*	運転中にホッパを取り外しての零点パラメータパケット	*/
/*								*/
/*--------------------------------------------------------------*/

typedef struct prr0129t {
	unsigned long	wh_nonactive;	/* WH non active head	*/
	unsigned long	ph_nonactive;	/* PH non active head	*/
} PRR0129T;

#if	defined(STORAGE)
struct prr0129t	prr0129s;
#else
#if	!defined(NOEXTERN)
extern struct prr0129t	prr0129s;
#endif
#endif

#endif
