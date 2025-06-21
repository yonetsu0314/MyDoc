/*======================================================================
 * File name	: pri0043.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	スパン係数応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0043.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0043.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi043.h 1.1 1997/01/08 08:56:00 chimura Exp
 *log: rzi043.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRI0043_H)
#define	PRI0043_H


/*------------------------------------------------------*/
/*	スパン係数応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0043t {
	unsigned char	pwb_id;			/* ＡＤＣボード識別番号	*/
	unsigned char	range;			/* 秤量			*/
	unsigned long	date;			/* 調整日付		*/
	unsigned long	time;			/* 調整時間		*/
	unsigned short	span_coeffi[24];	/* ＳＰＡＮ係数		*/
} PRI0043T;

#if	defined(STORAGE)
struct pri0043t	pri0043s;
#else
#if	!defined(NOEXTERN)
extern struct pri0043t	pri0043s;
#endif
#endif

#endif
