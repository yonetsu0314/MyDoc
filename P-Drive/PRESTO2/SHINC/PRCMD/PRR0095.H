/*======================================================================
 * File name    : prr0095.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0095.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0095.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr095.h 1.1 1997/12/04 13:17:50
 *Log: rzr095.h
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR0095_H)
#define	PRR0095_H


/*------------------------------------------------------*/
/*	マルチ連動拡張パラメータ構造						*/
/*														*/
/*------------------------------------------------------*/

typedef struct prr0095t {
	unsigned char	s_cmd;			/* sub command		*/
	unsigned char	p1;				/* parm1			*/
	unsigned char	p2;				/* parm2			*/
	unsigned char	p3;				/* parm3			*/
	unsigned char	p4;				/* parm4			*/
} PRR0095T;

#if	defined(STORAGE)
struct prr0095t	prr0095s;
#else
#if	!defined(NOEXTERN)
extern struct prr0095t	prr0095s;
#endif
#endif

#endif
