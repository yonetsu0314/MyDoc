/*======================================================================
 * File name    : rzi043.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi043.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi043.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi043.h 1.1 1997/01/08 08:56:00 chimura
 *Log: rzi043.h
 * リビジョン 1.1  1997/01/08  08:56:00  chimura
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI043_H)
#define	RZI043_H


/*------------------------------------------------------*/
/*	スパン係数応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi043t {
	unsigned char	pwb_id;			/* ＡＤＣボード識別番号	*/
	unsigned long	date;			/* 調整日付		*/
	unsigned long	time;			/* 調整時間		*/
	unsigned short	span_coeffi[16];	/* ＳＰＡＮ係数		*/
} RZI043T;

#if	defined(STORAGE)
struct rzi043t	rzi043s;
#else
#if	!defined(NOEXTERN)
extern struct rzi043t	rzi043s;
#endif
#endif

#endif
