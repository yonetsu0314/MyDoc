/*======================================================================
 * File name    : rzr095.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr095.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr095.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr095.h 1.1 1997/12/04 13:17:50 itami
 *Log: rzr095.h
 * リビジョン 1.1  1997/12/04  13:17:50  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR095_H)
#define	RZR095_H


/*------------------------------------------------------*/
/*	マルチ連動拡張パラメータ構造			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr095t {
	unsigned char	s_cmd;			/* sub command		*/
	unsigned char	p1;			/* parm1		*/
	unsigned char	p2;			/* parm2		*/
	unsigned char	p3;			/* parm3		*/
	unsigned char	p4;			/* parm4		*/
} RZR095T;

#if	defined(STORAGE)
struct rzr095t	rzr095s;
#else
#if	!defined(NOEXTERN)
extern struct rzr095t	rzr095s;
#endif
#endif

#endif
