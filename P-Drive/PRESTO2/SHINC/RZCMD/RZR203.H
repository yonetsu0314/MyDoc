/*======================================================================
 * File name	: rzr203.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 * 計量機の構成仕様拡張テキスト構造(assistant feeder)
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr203.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr203.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr203.h 1.1 1970/01/01 00:00:00 kawamura
 *Log: rzr203.h
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(RZR203_H)
#define	RZR203_H

/*------------------------------------------------------*/
/*	計量機の構成仕様拡張テキスト構造		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr203t {
	unsigned char	nof_af;			/* number of AF		*/
	unsigned long	af_assign_ptn[32];	/* AF loacate pattern
						[nof_af]		*/
} RZR203T;

#if	defined(STORAGE)
struct rzr203t	rzr203s;
#else
#if	!defined(NOEXTERN)
extern struct rzr203t	rzr203s;
#endif
#endif

#endif
