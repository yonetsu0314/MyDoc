/*======================================================================
 * File name	: prr0115.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	全排出時の自動零点制御データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0115.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0115.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0115_H)
#define	PRR0115_H


enum prr01150e {			/* 自動零点制御する／しない	*/
	DRAIN_AUTO_ZERO_OFF,		/* 自動零点制御しない		*/
	DRAIN_AUTO_ZERO_ON,		/* 自動零点制御する		*/
};

/*------------------------------------------------------*/
/*	全排出時の自動零点制御パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/


typedef struct prr0115t {
	unsigned char	ch_id;		/* channel number		*/
	enum prr01150e	drain_zero;	/* ON/OFF			*/
} PRR0115T;

#if	defined(STORAGE)
struct prr0115t	prr0115s;
#else
#if	!defined(NOEXTERN)
extern struct prr0115t	prr0115s;
#endif
#endif

#endif
